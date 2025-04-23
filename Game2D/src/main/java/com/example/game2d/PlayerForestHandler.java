package com.example.game2d;

import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.physics.CollisionHandler;

public class PlayerForestHandler extends CollisionHandler {

    public PlayerForestHandler() {
        super(EntityType.PLAYER, EntityType.FOREST);
    }

    @Override
    protected void onCollisionBegin(Entity player, Entity forest) {
        Entity keyEntity = forest.getObject("keyEntity");
        if (!keyEntity.isActive()) {
            keyEntity.setProperty("activated", false);
            getGameWorld().addEntity(keyEntity);
        }
        keyEntity.setOpacity(1);
    }

    @Override
    protected void onCollisionEnd(Entity player, Entity forest) {
        Entity keyEntity = forest.getObject("keyEntity");
        if (!keyEntity.getBoolean("activated")) {
            keyEntity.setOpacity(0);
        }

    }
}
