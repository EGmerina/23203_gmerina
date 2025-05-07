package com.example.game2d;

import com.almasb.fxgl.animation.Interpolators;
import com.almasb.fxgl.app.GameApplication;
import com.almasb.fxgl.app.GameSettings;
import com.almasb.fxgl.app.scene.FXGLMenu;
import com.almasb.fxgl.app.scene.LoadingScene;
import com.almasb.fxgl.app.scene.SceneFactory;
import com.almasb.fxgl.app.services.FXGLAssetLoaderService;
import com.almasb.fxgl.core.math.Vec2;
import com.almasb.fxgl.core.util.LazyValue;
import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.dsl.FXGLForKtKt;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.SpawnData;
import com.almasb.fxgl.entity.level.Level;
import com.almasb.fxgl.entity.level.tiled.TMXLevelLoader;
import com.almasb.fxgl.entity.level.tiled.TiledMap;
import com.almasb.fxgl.input.UserAction;
import com.almasb.fxgl.io.FileSystemService;
import com.almasb.fxgl.pathfinding.CellMoveComponent;
import com.almasb.fxgl.pathfinding.CellState;
import com.almasb.fxgl.pathfinding.astar.AStarGrid;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;
import com.almasb.fxgl.physics.CollisionHandler;
import com.almasb.fxgl.physics.PhysicsComponent;
import com.almasb.fxgl.ui.UI;
import javafx.animation.Interpolator;
import javafx.beans.property.IntegerProperty;
import javafx.geometry.Point2D;
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

    private static final int MAX_LEVEL = 2;
    private static final int STARTING_LEVEL = 0;


    private PlayerComponent playerComponent;
    private AStarGrid grid;
    private AStarGrid runnerGrid;


    private LazyValue<LevelEndScene> levelEndScene = new LazyValue<>(() -> new LevelEndScene());

    private Entity player;

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

        nextLevel();


        set("player", player);

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

        Entity cameraEntity = FXGL.entityBuilder()
                .with(new CameraComponent(getGameWorld().getSingleton(PLAYER), 0.1)) // 0.1 - коэффициент плавности
                .buildAndAttach();

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

        onCollisionOneTimeOnly(PLAYER, END, (player, end) -> {
            levelEndScene.get().onLevelFinish();

            FXGL.getGameScene().getViewport().fade(() -> {
                nextLevel();
            });
        });

        FXGL.getPhysicsWorld().addCollisionHandler(new CollisionHandler(GameType.PLAYER, GameType.SEA) {
            @Override
            protected void onCollision(Entity player, Entity sea) {
                FXGL.getDialogService().showMessageBox("Game Over! You drowned!", () -> {
                    FXGL.getGameController().gotoMainMenu();
                });
            }
        });
        FXGL.getPhysicsWorld().addCollisionHandler(new CollisionHandler(GameType.PLAYER, PUDDLE) {
            @Override
            protected void onCollision(Entity player, Entity puddle) {
                CellMoveComponent moveComponent = player.getComponent(CellMoveComponent.class);
                moveComponent.setSpeed(100);
                FXGL.runOnce(() -> {
                    moveComponent.setSpeed(180);
                }, Duration.seconds(4));
            }
        });
        FXGL.getPhysicsWorld().addCollisionHandler(new CollisionHandler(GameType.PLAYER, START) {
            @Override
            protected void onCollision(Entity player, Entity start) {
                getGameScene().getViewport().setZoom(1);
            }

            @Override
            protected void onCollisionEnd(Entity player, Entity start) {
                getGameScene().getViewport().setZoom(2);
            }
        });
    }

    @Override
    protected void initGameVars(Map<String, Object> vars) {
        vars.put("time", TIME_PER_LEVEL);
        vars.put("score", 0);
        vars.put("level", STARTING_LEVEL);
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

    private void setLevel(int levelNum) {

        set("levelTime", 0.0);

        Level level = setLevelFromMap("tmx/level" + levelNum + ".tmx");

        playerComponent = getGameWorld().getSingleton(PLAYER).getComponent(PlayerComponent.class);
        playerComponent.getEntity().addComponent(new AStarMoveComponent(grid));
        Entity cameraEntity = FXGL.entityBuilder()
                .with(new CameraComponent(getGameWorld().getSingleton(PLAYER), 0.1)) // 0.1 - коэффициент плавности
                .buildAndAttach();
        player = getGameWorld().getSingleton(PLAYER);


        var shortestTime = level.getProperties().getDouble("star1time");

        var levelTimeData = new LevelEndScene.LevelTimeData(shortestTime * 2.4, shortestTime * 1.3, shortestTime);

        set("levelTimeData", levelTimeData);
    }

    private void nextLevel() {
        if (geti("level") == MAX_LEVEL) {
            FXGL.getDialogService().showMessageBox("You finished the game!", () -> {
                FXGL.getGameController().gotoMainMenu();
            });
            return;
        }
        inc("level", +1);

        setLevel(geti("level"));

    }


    public static void main(String[] args) {
        launch(args);
    }
}
