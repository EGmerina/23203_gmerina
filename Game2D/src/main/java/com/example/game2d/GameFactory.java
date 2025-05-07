package com.example.game2d;

import com.almasb.fxgl.core.util.LazyValue;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.EntityFactory;
import com.almasb.fxgl.entity.SpawnData;
import com.almasb.fxgl.entity.Spawns;
import com.almasb.fxgl.entity.components.CollidableComponent;
import com.almasb.fxgl.entity.components.IrremovableComponent;
import com.almasb.fxgl.pathfinding.CellMoveComponent;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;
import com.almasb.fxgl.physics.BoundingShape;
import com.almasb.fxgl.physics.HitBox;
import com.almasb.fxgl.texture.AnimatedTexture;
import com.almasb.fxgl.texture.Texture;
//import com.example.game2d.components.ai.GuardCoinComponent;
import javafx.geometry.Point2D;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.util.Duration;

import javax.lang.model.element.Element;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static com.almasb.fxgl.dsl.FXGL.*;
import static com.example.game2d.GameApp.BLOCK_SIZE;
import static com.example.game2d.GameType.*;


public class GameFactory implements EntityFactory {


    @Spawns("player")
    public Entity newPlayer(SpawnData data) {
        System.out.println("spawn player");
        return entityBuilder(data)
                .type(PLAYER)
                .bbox(new HitBox(new Point2D(4, 4), BoundingShape.box(16, 32)))
                .anchorFromCenter()
                .with(new CollidableComponent(true))
                .with(new CellMoveComponent(BLOCK_SIZE, BLOCK_SIZE, 180).allowRotation(false))
                .with(new AStarMoveComponent(new LazyValue<>(() -> geto("grid"))))
                .with(new PlayerComponent())
                .build();
    }

    @Spawns("runner")
    public Entity newRunner(SpawnData data) {
        System.out.println("runner");
        return entityBuilder(data)
                .type(RUNNER)
                .bbox(new HitBox(new Point2D(4, 4), BoundingShape.box(16, 32)))
                .anchorFromCenter()
                .with(new CollidableComponent(true))
                .with(new CellMoveComponent(BLOCK_SIZE, BLOCK_SIZE, 100).allowRotation(false))
                .with(new AStarMoveComponent(new LazyValue<>(() -> geto("runnerGrid"))))
                .with(new RunnerComponent())
                .build();
    }

    @Spawns("tree")
    public Entity newTree(SpawnData data) {
        int width = data.get("width");
        int height = data.get("height");
        return entityBuilder(data)
                .type(TREE)
                .with(new CollidableComponent(true))
                .bbox(new HitBox(new Point2D(16, 32), BoundingShape.box(width/2, height/4)))
                .zIndex(100)
                .build();
    }


//    private Point2D[] parsePolygonPoints(String pointsStr) {
//        String[] points = pointsStr.split(" ");
//        Point2D[] vertices = new Point2D[points.length];
//
//        for (int i = 0; i < points.length; i++) {
//            String[] xy = points[i].split(",");
//            double x = Double.parseDouble(xy[0]);
//            double y = Double.parseDouble(xy[1]);
//            vertices[i] = new Point2D(x, y);
//        }
//
//        return vertices;
//    }

    @Spawns("trail")
    public Entity newTrail(SpawnData data) {
        System.out.println("trail");
        int width = data.get("width");
        int height = data.get("height");
        return entityBuilder(data)
                .type(TRAIL)
                .bbox(new HitBox(BoundingShape.box(width, height)))
                .build();
    }

    @Spawns("sea")
    public Entity newSea(SpawnData data) {
        int width = data.get("width");
        int height = data.get("height");
        return entityBuilder(data)
                .type(SEA)
                .with(new CollidableComponent(true))
                .bbox(new HitBox(BoundingShape.box(width, height)))
                .build();
    }

    @Spawns("puddle")
    public Entity newPuddle(SpawnData data) {
        int width = data.get("width");
        int height = data.get("height");
        return entityBuilder(data)
                .type(PUDDLE)
                .with(new CollidableComponent(true))
                .bbox(new HitBox(BoundingShape.box(width, height)))
                .build();
    }

    @Spawns("start")
    public Entity newStart(SpawnData data) {
        int width = data.get("width");
        int height = data.get("height");
        return entityBuilder(data)
                .type(START)
                .with(new CollidableComponent(true))
                .bbox(new HitBox(BoundingShape.box(width, height)))
                .build();
    }

    @Spawns("end")
    public Entity newEnd(SpawnData data) {
        int width = data.get("width");
        int height = data.get("height");
        return entityBuilder(data)
                .type(END)
                .with(new CollidableComponent(true))
                .bbox(new HitBox(BoundingShape.box(width, height)))
                .build();
    }

}
