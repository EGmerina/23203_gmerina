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

import static com.almasb.fxgl.dsl.FXGLForKtKt.*;
import static com.example.game2d.EntityType.*;

public class GameFactory implements EntityFactory {
    @Spawns("background")
    public Entity newBackground(SpawnData data) {
        return entityBuilder()
                .view(new ScrollingBackgroundView(texture("background/forest.png")
                        .getImage(), getAppWidth(), getAppHeight()))
                .zIndex(-1)
                .with(new IrremovableComponent())
                .build();
    }

    @Spawns("forest")
    public Entity newForest(SpawnData data) {
        return entityBuilder(data)
                .type(FOREST)
                .bbox(new HitBox(BoundingShape.
                        box(data.<Integer>get("width"),
                                data.<Integer>get("height"))))
                .with(new PhysicsComponent())
                .build();
    }

    @Spawns("player")
    public Entity newPlayer(SpawnData data){
        PhysicsComponent physics = new PhysicsComponent();
        physics.setBodyType(BodyType.DYNAMIC);
        return entityBuilder(data)
                .type(PLAYER)
                .bbox(new HitBox(new Point2D(10, 25),
                        BoundingShape.box(10, 17)))
                .with(physics)
                .with(new CollidableComponent(true))
                .with(new IrremovableComponent())
                .with(new PlayerComponent())
                .build();
    }

    @Spawns("runner")
    public Entity newRunner(SpawnData data){
        PhysicsComponent physics = new PhysicsComponent();
        physics.setBodyType(BodyType.DYNAMIC);
        return entityBuilder(data)
                .type(RUNNER)
                .bbox(new HitBox(new Point2D(10, 25),
                        BoundingShape.box(10, 17)))
                .with(physics)
                .with(new CollidableComponent(true))
                .with(new RunnerComponent())
                .build();
    }

    @Spawns("marker")
    public Entity newExitTrigger(SpawnData data) {
        return entityBuilder(data)
                .type(MARKER)
                .bbox(new HitBox(BoundingShape.
                        box(data.<Integer>get("width"),
                                data.<Integer>get("height"))))
                .with(new CollidableComponent(true))
                .build();
    }
}
