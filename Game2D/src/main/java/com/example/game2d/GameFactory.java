package com.example.game2d;

import com.almasb.fxgl.core.util.LazyValue;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.EntityFactory;
import com.almasb.fxgl.entity.SpawnData;
import com.almasb.fxgl.entity.Spawns;
import com.almasb.fxgl.entity.components.CollidableComponent;
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

import static com.almasb.fxgl.dsl.FXGL.*;
import static com.example.game2d.GameApp.BLOCK_SIZE;
import static com.example.game2d.GameType.*;


public class GameFactory implements EntityFactory {


    @Spawns("player")
    public Entity newPlayer(SpawnData data) {
        return entityBuilder(data)
                .type(PLAYER)
                .bbox(new HitBox(new Point2D(4, 4), BoundingShape.box(16, 32)))
                .anchorFromCenter()
                .with(new CollidableComponent(true))
                .with(new CellMoveComponent(BLOCK_SIZE, BLOCK_SIZE, 200).allowRotation(false))
                .with(new AStarMoveComponent(new LazyValue<>(() -> geto("grid"))))
                .with(new PlayerComponent())
                .build();
    }

    @Spawns("runner")
    public Entity newRunner(SpawnData data) {
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
        return entityBuilder(data)
                .type(TREE)
                .with(new CollidableComponent(true))
                .bbox(new HitBox(new Point2D(32, 40), BoundingShape.box(10, 30)))
                .anchorFromCenter()
                .zIndex(100)
                .build();
    }

    @Spawns("trail")
    public Entity newTrail(SpawnData data) {
        int width = data.get("width");
        int height = data.get("height");
        return entityBuilder(data)
                .type(TRAIL)
                .bbox(new HitBox(BoundingShape.box(width, height)))
                .build();
    }

}
