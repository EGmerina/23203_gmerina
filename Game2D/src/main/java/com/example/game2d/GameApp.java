package com.example.game2d;

import com.almasb.fxgl.animation.Interpolators;
import com.almasb.fxgl.app.GameApplication;
import com.almasb.fxgl.app.GameSettings;
import com.almasb.fxgl.app.scene.FXGLMenu;
import com.almasb.fxgl.app.scene.SceneFactory;
import com.almasb.fxgl.core.math.Vec2;
import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.SpawnData;
import com.almasb.fxgl.input.UserAction;
import com.almasb.fxgl.pathfinding.CellState;
import com.almasb.fxgl.pathfinding.astar.AStarGrid;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;
import com.almasb.fxgl.physics.CollisionHandler;
import javafx.scene.input.KeyCode;
import javafx.scene.paint.Color;
import javafx.scene.shape.Polyline;
import javafx.util.Duration;

import java.util.List;
import java.util.stream.Collectors;

import static com.almasb.fxgl.dsl.FXGL.*;
import static com.example.game2d.GameType.*;


public class GameApp extends GameApplication {

    public static final int BLOCK_SIZE = 32;

    private PlayerComponent playerComponent;
    private AStarGrid grid;

    @Override
    protected void initSettings(GameSettings settings) {
        settings.setWidth(30 * 32);
        settings.setHeight(30 * 32);
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

        setLevelFromMap("tmx/level1.tmx");

        getGameWorld().getEntities().forEach(e -> {
            if (e.getProperties().exists("zIndex")) {
                int z = e.getInt("zIndex");
                e.getViewComponent().setZIndex(z);
            }
        });
        grid = AStarGrid.fromWorld(getGameWorld(), 30, 30, BLOCK_SIZE, BLOCK_SIZE, (type) -> {
            if (type == TRAIL)
                return CellState.NOT_WALKABLE;

            return CellState.WALKABLE;
        });
        set("grid", grid);
        var waypoints = FXGL.getGameWorld().getObjectGroup("waypoints")
                .getObject("way1")
                .getPolyline(); // получаем точки полилинии

        // Преобразуем в список Vec2 (если нужно)
        List<Vec2> path = waypoints.getPoints().stream()
                .map(p -> Vec2.fromPixels(p.x, p.y))
                .collect(Collectors.toList());

        playerComponent = getGameWorld().getSingleton(PLAYER).getComponent(PlayerComponent.class);
        playerComponent.getEntity().addComponent(new AStarMoveComponent(grid));
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
    }

    public static void main(String[] args) {
        launch(args);
    }
}
