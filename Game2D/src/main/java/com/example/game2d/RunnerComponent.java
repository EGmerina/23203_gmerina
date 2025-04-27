package com.example.game2d;

import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.pathfinding.CellMoveComponent;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;

public class RunnerComponent extends Component {
    private AStarMoveComponent astar;
    private CellMoveComponent moveComponent;
    private int PLAYER_SPACE_RADIOUS = 4;

    @Override
    public void onUpdate(double tpf) {
        catchUp();
    }

    private void catchUp() {
        var player = FXGL.getGameWorld().getSingleton(GameType.PLAYER);

        int playerX = player.call("getCellX");
        int playerY = player.call("getCellY");

        var myX = moveComponent.getCellX();
        var myY = moveComponent.getCellY();

        if (Math.pow(myX - playerX, 2) + Math.pow(myY - playerY, 2) < PLAYER_SPACE_RADIOUS) {
            astar.stopMovement();
            return;
        }
        astar.moveToCell(playerX, playerY);

    }
}

