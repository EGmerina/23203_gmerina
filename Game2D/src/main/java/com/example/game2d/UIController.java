package com.example.game2d;

import com.almasb.fxgl.ui.ProgressBar;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;

import static com.almasb.fxgl.dsl.FXGL.getUIFactoryService;
import static com.almasb.fxgl.dsl.FXGL.getip;

public class UIController implements com.almasb.fxgl.ui.UIController {

    @FXML
    private Pane root;

    private ProgressBar timeBar;

    @FXML
    private Label labelScore;

    @FXML
    private Label labelTime;

    @Override
    public void init() {
        timeBar = new ProgressBar(false);
        timeBar.setHeight(50);
        timeBar.setTranslateX(-60);
        timeBar.setTranslateY(100);
        timeBar.setRotate(-90);
        timeBar.setFill(Color.GREEN);
        timeBar.setLabelVisible(false);
        timeBar.setMaxValue(GameApp.TIME_PER_LEVEL);
        timeBar.setMinValue(0);
        timeBar.setCurrentValue(GameApp.TIME_PER_LEVEL);
        timeBar.currentValueProperty().bind(getip("countdown"));

        root.getChildren().addAll(timeBar);

        labelScore.setFont(getUIFactoryService().newFont(18));
        labelTime.setFont(getUIFactoryService().newFont(12));

        labelScore.textProperty().bind(getip("score").asString("Score:\n%d"));
        labelTime.textProperty().bind(getip("time").asString("Time:\n%d sec"));

    }
}
