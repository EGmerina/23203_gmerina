package com.example.game2d.components.ai;

import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.entity.component.Required;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;
import com.example.game2d.GameType;


@Required(AStarMoveComponent.class)
public class DelayedChasePlayerComponent extends Component {

    private AStarMoveComponent astar;

    private boolean isDelayed = false;

    @Override
    public void onUpdate(double tpf) {
        if (!isDelayed) {
            move();
        } else {

            // if delayed, only move when reached destination
            if (astar.isAtDestination()) {
                move();
            }
        }
    }

    private void move() {
        var player = FXGL.getGameWorld().getSingleton(GameType.PLAYER);

        int x = player.call("getCellX");
        int y = player.call("getCellY");

        astar.moveToCell(x, y);
    }

    public DelayedChasePlayerComponent withDelay() {
        isDelayed = true;
        return this;
    }
}
