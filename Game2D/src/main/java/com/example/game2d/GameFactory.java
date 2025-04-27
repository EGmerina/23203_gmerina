package com.example.game2d;

import com.almasb.fxgl.core.util.LazyValue;
import com.almasb.fxgl.dsl.components.RandomAStarMoveComponent;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.EntityFactory;
import com.almasb.fxgl.entity.SpawnData;
import com.almasb.fxgl.entity.Spawns;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.entity.components.CollidableComponent;
import com.almasb.fxgl.pathfinding.CellMoveComponent;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;
import com.almasb.fxgl.physics.BoundingShape;
import com.almasb.fxgl.physics.HitBox;
import com.almasb.fxgl.texture.AnimatedTexture;
import com.almasb.fxgl.texture.Texture;
import com.example.game2d.components.PaletteChangingComponent;
import com.example.game2d.components.PlayerComponent;
import com.example.game2d.components.ai.DelayedChasePlayerComponent;
//import com.example.game2d.components.ai.GuardCoinComponent;
import javafx.geometry.Point2D;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.util.Duration;

import java.util.Map;
import java.util.function.Supplier;

import static com.almasb.fxgl.dsl.FXGL.*;
import static com.example.game2d.GameApp.BLOCK_SIZE;
import static com.example.game2d.GameType.*;


public class GameFactory implements EntityFactory {

//    @Spawns("56")
//    public Entity newBlock(SpawnData data) {
//        var rect = new Rectangle(38, 38, Color.BLACK);
//        rect.setArcWidth(15);
//        rect.setArcHeight(15);
//        rect.setStrokeWidth(1);
//        rect.setStroke(Color.BLUE);
//
//        return entityBuilder(data)
//                .type(BLOCK)
//                .viewWithBBox(rect)
//                .zIndex(-1)
//                .build();
//    }
//
//    @Spawns("35")
//    public Entity newCoin(SpawnData data) {
//
//        return entityBuilder(data)
//                .type(COIN)
//                .bbox(new HitBox(new Point2D(5, 5), BoundingShape.box(30, 30)))
//                .zIndex(-1)
//                .with(new CollidableComponent(true))
//                .with(new CellMoveComponent(BLOCK_SIZE, BLOCK_SIZE, 50))
//                .scale(0.5, 0.5)
//                .build();
//    }

    @Spawns("player")
    public Entity newPlayer(SpawnData data) {
        AnimatedTexture view = texture("player.png").toAnimatedTexture(2, Duration.seconds(0.33));

        return entityBuilder(data)
                .type(PLAYER)
                .bbox(new HitBox(new Point2D(4, 4), BoundingShape.box(32, 32)))
                .anchorFromCenter()
                .view(view.loop())
                .with(new CollidableComponent(true))
                .with(new CellMoveComponent(BLOCK_SIZE, BLOCK_SIZE, 200).allowRotation(true))
                // there is no grid constructed yet, so pass lazily
                .with(new AStarMoveComponent(new LazyValue<>(() -> geto("grid"))))
                .with(new PlayerComponent())
                .rotationOrigin(35 / 2.0, 40 / 2.0)
                .build();
    }

    @Spawns("tree")
    public Entity newTree(SpawnData data) {
        Texture view = texture("tree_2.png");
        return entityBuilder(data)
                .type(TREE)
                .with(new CollidableComponent(true))
                .bbox(new HitBox(new Point2D(10, 10), BoundingShape.box(10, 30)))
                .anchorFromCenter()
                .zIndex(100)
                .build();
    }
    @Spawns("trail")
    public Entity newTrail(SpawnData data) {
        return entityBuilder(data)
                .type(TRAIL)
                .build();
    }

}
