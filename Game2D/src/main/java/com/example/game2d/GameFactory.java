package com.example.game2d;

import com.almasb.fxgl.dsl.views.ScrollingBackgroundView;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.EntityFactory;
import com.almasb.fxgl.entity.SpawnData;
import com.almasb.fxgl.entity.Spawns;
import com.almasb.fxgl.entity.components.CollidableComponent;
import com.almasb.fxgl.entity.components.IrremovableComponent;
import com.almasb.fxgl.physics.BoundingShape;
import com.almasb.fxgl.physics.HitBox;
import com.almasb.fxgl.physics.PhysicsComponent;
import com.almasb.fxgl.physics.box2d.dynamics.BodyType;
import javafx.geometry.Point2D;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

import static com.almasb.fxgl.dsl.FXGLForKtKt.*;
import static com.example.game2d.EntityType.*;

public class GameFactory implements EntityFactory {

    @Spawns("1")
    public Entity newBlock(SpawnData data) {
        var rect = new Rectangle(38, 38, Color.BLACK);
        rect.setArcWidth(25);
        rect.setArcHeight(25);
        rect.setStrokeWidth(1);
        rect.setStroke(Color.BLUE);

        return entityBuilder(data)
                .type(TRAIL)
                .viewWithBBox(rect)
                .zIndex(-1)
                .build();
    }

//    @Spawns("player")
//    public Entity newPlayer(SpawnData data) {
//        PhysicsComponent physics = new PhysicsComponent();
//        physics.setBodyType(BodyType.DYNAMIC);
//        return entityBuilder(data)
//                .type(PLAYER)
//                .bbox(new HitBox(new Point2D(10, 25),
//                        BoundingShape.box(10, 17)))
//                .with(physics)
//                .with(new IrremovableComponent())
//                .with(new PlayerComponent())
//                .build();
//    }

}
