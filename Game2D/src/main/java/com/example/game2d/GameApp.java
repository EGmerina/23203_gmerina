package com.example.game2d;

import com.almasb.fxgl.animation.Interpolators;
import com.almasb.fxgl.app.GameApplication;
import com.almasb.fxgl.app.GameSettings;
import com.almasb.fxgl.app.scene.FXGLMenu;
import com.almasb.fxgl.app.scene.LoadingScene;
import com.almasb.fxgl.app.scene.SceneFactory;
import com.almasb.fxgl.app.services.FXGLAssetLoaderService;
import com.almasb.fxgl.core.math.Vec2;
import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.SpawnData;
import com.almasb.fxgl.entity.level.Level;
import com.almasb.fxgl.entity.level.tiled.TMXLevelLoader;
import com.almasb.fxgl.entity.level.tiled.TiledMap;
import com.almasb.fxgl.input.UserAction;
import com.almasb.fxgl.io.FileSystemService;
import com.almasb.fxgl.pathfinding.CellState;
import com.almasb.fxgl.pathfinding.astar.AStarGrid;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;
import com.almasb.fxgl.physics.CollisionHandler;
import com.almasb.fxgl.ui.UI;
import javafx.animation.Interpolator;
import javafx.beans.property.IntegerProperty;
import javafx.scene.effect.ColorAdjust;
import javafx.scene.input.KeyCode;
import javafx.scene.paint.Color;
import javafx.scene.shape.Polyline;
import javafx.util.Duration;

import java.net.URL;
import java.nio.file.Paths;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import static com.almasb.fxgl.dsl.FXGL.*;
import static com.almasb.fxgl.dsl.FXGLForKtKt.getGameScene;
import static com.example.game2d.GameType.*;


public class GameApp extends GameApplication {

    public static final int BLOCK_SIZE = 32;
    public static final int TIME_PER_LEVEL = 50;
    public static final int MAP_SIZE = 30;
    private static final int UI_SIZE = 80;

    private PlayerComponent playerComponent;
    private AStarGrid grid;
    private AStarGrid runnerGrid;

//    private boolean isNight = false;
//    private ColorAdjust nightEffect = new ColorAdjust();

    @Override
    protected void initSettings(GameSettings settings) {
        settings.setWidth(MAP_SIZE * BLOCK_SIZE + UI_SIZE);
        settings.setHeight(MAP_SIZE * BLOCK_SIZE);
        settings.setTitle("Game 2D");
        settings.setVersion("1.0");
        settings.getCSSList().add("main.css");
        settings.setMainMenuEnabled(true);
        settings.setSceneFactory(new SceneFactory() {
            @Override
            public FXGLMenu newMainMenu() {
                return new GameMainMenu();
            }
        });
    }

    @Override
    protected void initInput() {

        getInput().addAction(new UserAction("Left") {
            @Override
            protected void onAction() {
                playerComponent.left();
            }

            @Override
            protected void onActionEnd() {
                playerComponent.stop();
            }
        }, KeyCode.LEFT);

        getInput().addAction(new UserAction("Right") {
            @Override
            protected void onAction() {
                playerComponent.right();
            }

            @Override
            protected void onActionEnd() {
                playerComponent.stop();
            }
        }, KeyCode.RIGHT);

        getInput().addAction(new UserAction("Down") {
            @Override
            protected void onAction() {
                playerComponent.down();
            }

            @Override
            protected void onActionEnd() {
                playerComponent.stop();
            }
        }, KeyCode.DOWN);

        getInput().addAction(new UserAction("Up") {
            @Override
            protected void onAction() {
                playerComponent.up();
            }

            @Override
            protected void onActionEnd() {
                playerComponent.stop();
            }
        }, KeyCode.UP);

    }

    @Override
    protected void initGame() {
        getGameScene().setBackgroundColor(Color.DARKGREEN);

        getGameWorld().addEntityFactory(new GameFactory());

        setLevelFromMap("tmx/level2.tmx");


        grid = AStarGrid.fromWorld(getGameWorld(), MAP_SIZE, MAP_SIZE, BLOCK_SIZE, BLOCK_SIZE, (type) -> {
            return CellState.WALKABLE;
        });
        set("grid", grid);

        runnerGrid = AStarGrid.fromWorld(getGameWorld(), MAP_SIZE, MAP_SIZE, BLOCK_SIZE, BLOCK_SIZE, (type) -> {
            if (type == TRAIL || type == START || type == END)
                return CellState.WALKABLE;

            return CellState.NOT_WALKABLE;
        });
        set("runnerGrid", runnerGrid);

        playerComponent = getGameWorld().getSingleton(PLAYER).getComponent(PlayerComponent.class);
        playerComponent.getEntity().addComponent(new AStarMoveComponent(grid));

        run(() -> inc("time", -1), Duration.seconds(1));

        getWorldProperties().<Integer>addListener("time", (old, now) -> {
            if (now == 0) {
                FXGL.getDialogService().showMessageBox("Game Over! Time is up!", () -> {
                    FXGL.getGameController().gotoMainMenu();
                });
            }
        });

        //getGameScene().getViewport().bindToEntity(getGameWorld().getSingleton(PLAYER), getAppWidth()/2, getAppHeight()/2);
        Entity cameraEntity = FXGL.entityBuilder()
                .with(new CameraComponent(getGameWorld().getSingleton(PLAYER), 0.1)) // 0.1 - коэффициент плавности
                .buildAndAttach();

//        nightEffect.setBrightness(0);  // Начальная яркость (день)
//        FXGL.getGameWorld().getEntities().forEach(entity -> {
//            entity.getViewComponent().getParent().setEffect(nightEffect);
//        });
//
//        // Таймер для смены дня и ночи (каждые 30 сек)
//        FXGL.run(() -> {
//            isNight = !isNight;
//            double targetBrightness = isNight ? -0.2 : 0.0;
//            FXGL.animationBuilder()
//                    .duration(Duration.seconds(3))
//                    .interpolator(Interpolator.EASE_BOTH)
//                    .animate(nightEffect.brightnessProperty())
//                    .from(nightEffect.getBrightness())
//                    .to(targetBrightness)
//                    .buildAndPlay();
//        }, Duration.seconds(25));


    }

    @Override
    protected void initPhysics() {
        FXGL.getPhysicsWorld().addCollisionHandler(new CollisionHandler(GameType.PLAYER, GameType.TREE) {
            @Override
            protected void onCollision(Entity player, Entity tree) {
                FXGL.getDialogService().showMessageBox("Game Over! You hit a tree!", () -> {
                    FXGL.getGameController().gotoMainMenu();
                });
            }
        });
        FXGL.getPhysicsWorld().addCollisionHandler(new CollisionHandler(GameType.PLAYER, GameType.END) {
            @Override
            protected void onCollision(Entity player, Entity tree) {
                FXGL.getDialogService().showMessageBox("Well done!!! You ran all the way to the end :)\nScore: " + FXGL.getWorldProperties().getInt("score"), () -> {
                    FXGL.getGameController().gotoMainMenu();
                });
            }
        });
        FXGL.getPhysicsWorld().addCollisionHandler(new CollisionHandler(GameType.PLAYER, GameType.SEA) {
            @Override
            protected void onCollision(Entity player, Entity sea) {
                FXGL.getDialogService().showMessageBox("Game Over! You drowned!", () -> {
                    FXGL.getGameController().gotoMainMenu();
                });
            }
        });
        FXGL.getPhysicsWorld().addCollisionHandler(new CollisionHandler(GameType.PLAYER, START) {
            @Override
            protected void onCollision(Entity player, Entity tree) {
                getGameScene().getViewport().setZoom(1);
            }

            @Override
            protected void onCollisionEnd(Entity player, Entity tree) {
                getGameScene().getViewport().setZoom(2);
            }
        });
    }

    @Override
    protected void initGameVars(Map<String, Object> vars) {
        vars.put("time", TIME_PER_LEVEL);
        vars.put("score", 0);
    }

    @Override
    protected void onUpdate(double tpf) {

        getWorldProperties().addListener("time", (oldTime, newTime) -> {
            int score = getWorldProperties().getValue("score");
            int time = getWorldProperties().getValue("time");
            score = score * (int) (time / 25.0);
            getWorldProperties().setValue("score", score);
        });

    }

    @Override
    protected void initUI() {
        UI ui = getAssetLoader().loadUI("game_ui.fxml", new UIController());
        ui.getRoot().setTranslateX(MAP_SIZE * BLOCK_SIZE);
        getGameScene().addUI(ui);
    }

    public static void main(String[] args) {
        launch(args);
    }
}
