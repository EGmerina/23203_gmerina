package com.example.game2d;

import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.component.Component;
import javafx.geometry.Point2D;

import static com.almasb.fxgl.dsl.FXGLForKtKt.*;

public class CameraComponent extends Component {

    private Entity player;
    private double smoothness;

    public CameraComponent(Entity target, double smoothness) {
        if (target == null) return;
        this.player = target;
        this.smoothness = smoothness;
    }

    @Override
    public void onUpdate(double tpf) {

        double cameraX = getGameScene().getViewport().getX();
        double cameraY = getGameScene().getViewport().getY();

//        double targetX = Math.min(Math.max(0, player.getX() - getAppWidth() / 2),
//                levelWidth - getAppWidth());
//        double targetY = Math.min(Math.max(0, player.getY() - getAppHeight() / 2),
//                levelHeight - getAppHeight());

        Point2D cameraPos = new Point2D(
                getGameScene().getViewport().getX(),
                getGameScene().getViewport().getY()
        );

        // Вычисляем целевую позицию (центрируем камеру на цели)
        Point2D targetPos = new Point2D(
                player.getX() - getGameScene().getViewport().getWidth() / 2,
                player.getY() - getGameScene().getViewport().getHeight() / 2
        );

        // Плавная интерполяция между текущей и целевой позицией
        Point2D newPos = cameraPos.interpolate(targetPos, smoothness * tpf * 60);


        getGameScene().getViewport().setX(newPos.getX());
        getGameScene().getViewport().setY(newPos.getY());

        getGameScene().getViewport().setZoom(2);
    }
}
