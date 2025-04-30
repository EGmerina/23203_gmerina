package com.example.game2d;

import com.almasb.fxgl.app.scene.FXGLMenu;
import com.almasb.fxgl.app.scene.MenuType;
import com.almasb.fxgl.dsl.FXGL;
import javafx.geometry.Pos;
import javafx.scene.control.Button;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import org.jetbrains.annotations.NotNull;

public class GameMainMenu extends FXGLMenu {
    public GameMainMenu() {
        super(MenuType.MAIN_MENU);
        var bg = new Rectangle(getAppWidth(), getAppHeight(), Color.LIGHTGREEN);
        var startButton = new Button("Play");
        startButton.getStyleClass().add("menu_button");
        startButton.setOnAction(e -> fireNewGame());

        var exitButton = new Button("Exit");
        exitButton.getStyleClass().add("menu_button");
        exitButton.setOnAction(e -> fireExit());

        var box = new VBox(10, startButton, exitButton);
        box.setAlignment(Pos.CENTER);


        StackPane root = new StackPane(box);
        root.setPrefSize(FXGL.getAppWidth(), FXGL.getAppHeight());  

        getContentRoot().getChildren().addAll(bg, root);

    }
}
