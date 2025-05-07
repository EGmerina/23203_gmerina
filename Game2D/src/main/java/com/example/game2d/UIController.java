package com.example.game2d;

import com.almasb.fxgl.ui.ProgressBar;
import javafx.fxml.FXML;
import javafx.scene.control.Label;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;

import static com.almasb.fxgl.dsl.FXGL.getUIFactoryService;
import static com.almasb.fxgl.dsl.FXGL.getip;
import static com.almasb.fxgl.dsl.FXGLForKtKt.getdp;

public class UIController implements com.almasb.fxgl.ui.UIController {

    @FXML
    private Pane root;

    private ProgressBar timeBar;

    @FXML
    private Label  labelLostRunners;

    @FXML
    private Label labelTime;

    @FXML
    private Label labelDistance;


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

        labelTime.setFont(getUIFactoryService().newFont(18));
        labelTime.textProperty().bind(getip("time").asString("Time:\n%d sec"));

        labelLostRunners.setFont(getUIFactoryService().newFont(14));
        labelLostRunners.textProperty().bind(getip("lostRunners").asString("Lost\nrunners:\n%d "));

        labelDistance.setFont(getUIFactoryService().newFont(14));
        labelDistance.textProperty().bind(getdp("distance").asString("distance:\n%.0f "));

    }
}
