package com.example.game2d;

import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.physics.CollisionHandler;

import static com.almasb.fxgl.dsl.FXGL.getGameWorld;

public class PlayerTreeHandler extends CollisionHandler {

    public PlayerTreeHandler() {
        super(GameType.PLAYER, GameType.TREE);
    }

    @Override
    protected void onCollisionBegin(Entity player, Entity tree) {
    }
}
