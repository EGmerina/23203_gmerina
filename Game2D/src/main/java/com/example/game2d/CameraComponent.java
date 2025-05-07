package com.example.game2d;

import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.component.Component;
import javafx.geometry.Point2D;

import static com.almasb.fxgl.dsl.FXGLForKtKt.*;

public class CameraComponent extends Component {

    private Entity player;
    private double smoothness;
    private int ZOOM = 2;
    private int levelHeight = 30 * 32;
    private int levelWidth = 30 * 32;

    public CameraComponent(Entity target, double smoothness) {
        if (target == null) return;
        this.player = target;
        this.smoothness = smoothness;
    }

    @Override
    public void onUpdate(double tpf) {

        double zoom = getGameScene().getViewport().getZoom();


        Point2D cameraPos = new Point2D(
                getGameScene().getViewport().getX(),
                getGameScene().getViewport().getY()
        );

        // Вычисляем границы
        double minX = getGameScene().getViewport().getWidth() / (2 * zoom);
        double minY = getGameScene().getViewport().getHeight() / (2 * zoom);
        double maxX = levelWidth - getGameScene().getViewport().getWidth() / (2 * zoom);
        double maxY = levelHeight - getGameScene().getViewport().getHeight() / (2 * zoom);


        double targetX = Math.min(Math.max(player.getX(), minX), maxX);
        double targetY = Math.min(Math.max(player.getY(), minY), maxY);



        Point2D targetPos = new Point2D(targetX - levelWidth / (2 * zoom) + 40/zoom, targetY - levelHeight / (2 * zoom));

        // Плавная интерполяция между текущей и целевой позицией
        Point2D newPos = cameraPos.interpolate(targetPos, smoothness * tpf * 60);


        getGameScene().getViewport().setX(newPos.getX());
        getGameScene().getViewport().setY(newPos.getY());


    }
}
