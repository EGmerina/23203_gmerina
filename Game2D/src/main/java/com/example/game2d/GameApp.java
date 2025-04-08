package com.example.game2d;

import com.almasb.fxgl.app.GameApplication;
import com.almasb.fxgl.app.GameSettings;
import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.Entity;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.input.KeyCode;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;
import javafx.stage.Stage;

import java.io.IOException;
import java.util.Map;

public class GameApp extends GameApplication {

    @Override
    protected void initSettings(GameSettings gameSettings) {
        gameSettings.setWidth(600);
        gameSettings.setHeight(600);
        gameSettings.setTitle("Basic Game App");
        gameSettings.setVersion("0.1");
    }

    private Entity player;

    @Override
    protected void initGame() {
        player = FXGL.entityBuilder()
                .at(300, 300)
                .view(new Rectangle(25, 25, Color.BLUE))
                .buildAndAttach();
    }

    @Override
    protected void initInput() {
        FXGL.onKey(KeyCode.RIGHT, () -> {
            player.translateX(5);
            FXGL.inc("pixelMoved", +5);
        });

        FXGL.onKey(KeyCode.LEFT, () -> {
            player.translateX(-5);
            FXGL.inc("pixelMoved", -5);
        });

        FXGL.onKey(KeyCode.UP, () -> {
            player.translateY(-5);
        });

        FXGL.onKey(KeyCode.DOWN, () -> {
            player.translateY(5);
        });

    }

    @Override
    protected void initUI() {
        Text textPixels = new Text();
        textPixels.setTranslateX(50);
        textPixels.setTranslateY(100);

        textPixels.textProperty().bind(FXGL.getWorldProperties().
                intProperty("pixelMoved").asString());

        FXGL.getGameScene().addUINode(textPixels);
    }

    @Override
    protected void initGameVars(Map<String, Object> vars) {
        vars.put("pixelMoved", 0);
    }

    public static void main(String[] args) {
        launch(args);
    }
}