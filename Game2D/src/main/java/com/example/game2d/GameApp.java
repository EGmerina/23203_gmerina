package com.example.game2d;

import com.almasb.fxgl.app.GameApplication;
import com.almasb.fxgl.app.GameSettings;
import com.almasb.fxgl.app.scene.LoadingScene;
import com.almasb.fxgl.app.scene.SceneFactory;
import com.almasb.fxgl.app.scene.Viewport;
import com.almasb.fxgl.core.util.LazyValue;
import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.level.Level;
import com.almasb.fxgl.input.UserAction;
import com.almasb.fxgl.input.virtual.VirtualButton;
import com.almasb.fxgl.physics.PhysicsComponent;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.geometry.Point2D;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.Map;

import static com.almasb.fxgl.dsl.FXGL.*;

public class GameApp extends GameApplication {


    public static final int BLOCK_SIZE = 40;

    public static final int MAP_SIZE = 20;

    private static final int UI_SIZE = 80;

    @Override
    protected void initSettings(GameSettings gameSettings) {
        gameSettings.setWidth(1300);
        gameSettings.setHeight(1000);
        gameSettings.setTitle("Game 2D");


    }


    @Override
    protected void initGame() {

        getGameWorld().addEntityFactory(new GameFactory());

//        player = null;
//
//        nextLevel();

        player = spawn("player", 50, 50);
        set("player", player);

        spawn("background");

        Viewport viewport = getGameScene().getViewport();
        viewport.setBounds(-1500, 0, 250 * 70, getAppHeight());
        viewport.bindToEntity(player, getAppWidth() / 2, getAppHeight() / 2);
        viewport.setLazy(true);

//        player = FXGL.entityBuilder()
//                .at(300, 300)
//                .view(new Rectangle(25, 25, Color.BLUE))
//                .buildAndAttach();
    }

    @Override
    protected void initInput() {
        getInput().addAction(new UserAction("Left") {
            @Override
            protected void onAction() {
                player.getComponent(PlayerComponent.class).left();
            }

            @Override
            protected void onActionEnd() {
                player.getComponent(PlayerComponent.class).stop();
            }
        }, KeyCode.A, VirtualButton.LEFT);

        getInput().addAction(new UserAction("Right") {
            @Override
            protected void onAction() {
                player.getComponent(PlayerComponent.class).right();
            }

            @Override
            protected void onActionEnd() {
                player.getComponent(PlayerComponent.class).stop();
            }
        }, KeyCode.D, VirtualButton.RIGHT);

    }

//    @Override
//    protected void initUI() {
//        Text textPixels = new Text();
//        textPixels.setTranslateX(50);
//        textPixels.setTranslateY(100);
//
//        textPixels.textProperty().bind(FXGL.getWorldProperties().
//                intProperty("pixelMoved").asString());
//
//        FXGL.getGameScene().addUINode(textPixels);
//    }

    @Override
    protected void initGameVars(Map<String, Object> vars) {
        vars.put("level", STARTING_LEVEL);
        vars.put("levelTime", 0.0);
    }

//    @Override
//    protected void initPhysics() {
//        getPhysicsWorld().addCollisionHandler(new PlayerForestHandler());
//    }

    @Override
    protected void onUpdate(double tpf) {
        inc("levelTime", tpf);

//        if (player.getY() > getAppHeight()) {
//            setLevel(geti("level"));
//        }
    }

//    private void nextLevel() {
//        if (geti("level") == MAX_LEVEL) {
//            showMessage("You finished the demo!");
//            return;
//        }
//        inc("level", +1);
//        setLevel(geti("level"));
//    }
//
//    private void setLevel(int levelNum) {
//        if (player != null) {
//            player.getComponent(PhysicsComponent.class).
//                    overwritePosition(new Point2D(50, 50));
//            player.setZIndex(Integer.MAX_VALUE);
//        }
//        set("levelTime", 0.0);
//        Level level = setLevelFromMap("tmx/level" + levelNum +
//                ".tmx");
//        var shortestTime = level.getProperties().
//                getDouble("star1time");
////        var levelTimeData = new LevelEndScene.LevelTimeData
////                (shortestTime * 2.4, shortestTime * 1.3, shortestTime);
////        set("levelTimeData", levelTimeData);
//    }

    public static void main(String[] args) {
        launch(args);
    }
}