package com.example.game2d;

import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;

public class RunnerComponent extends Component {
    private AStarMoveComponent astar;

    @Override
    public void onUpdate(double tpf) {
        run();
    }

    private void run() {
        var player = FXGL.getGameWorld().getSingleton(GameType.PLAYER);

        int x = player.call("getCellX");
        int y = player.call("getCellY");

        astar.moveToCell(x, y);
    }
}

