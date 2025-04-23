package com.example.game2d.components;

import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.entity.component.Required;
import com.almasb.fxgl.pathfinding.CellMoveComponent;
import com.almasb.fxgl.pathfinding.astar.AStarCell;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;

import static com.example.game2d.components.PlayerComponent.MoveDirection.*;


@Required(AStarMoveComponent.class)
public class PlayerComponent extends Component {

    enum MoveDirection {
        UP, RIGHT, DOWN, LEFT, STOP
    }

    private CellMoveComponent moveComponent;

    private AStarMoveComponent astar;

    private MoveDirection currentMoveDir = RIGHT;
    private MoveDirection nextMoveDir = RIGHT;

    public void up() {
        nextMoveDir = UP;
    }

    public void down() {
        nextMoveDir = DOWN;
    }

    public void left() {
        nextMoveDir = LEFT;
    }

    public void right() {
        nextMoveDir = RIGHT;
    }

    public void stop() {
        nextMoveDir = STOP;
    }

    @Override
    public void onUpdate(double tpf) {
        var x = moveComponent.getCellX();
        var y = moveComponent.getCellY();

        if (x == 0 && currentMoveDir == LEFT) {
            astar.stopMovementAt(astar.getGrid().getWidth() - 1, moveComponent.getCellY());
            return;

        } else if (x == astar.getGrid().getWidth() - 1 && currentMoveDir == RIGHT) {
            astar.stopMovementAt(0, moveComponent.getCellY());
            return;
        }

        if (astar.isMoving())
            return;

        switch (nextMoveDir) {
            case UP:
                if (astar.getGrid().getUp(x, y).filter(c -> c.getState().isWalkable()).isPresent())
                    currentMoveDir = nextMoveDir;
                break;
            case RIGHT:
                if (astar.getGrid().getRight(x, y).filter(c -> c.getState().isWalkable()).isPresent())
                    currentMoveDir = nextMoveDir;
                break;
            case DOWN:
                if (astar.getGrid().getDown(x, y).filter(c -> c.getState().isWalkable()).isPresent())
                    currentMoveDir = nextMoveDir;
                break;
            case LEFT:
                if (astar.getGrid().getLeft(x, y).filter(c -> c.getState().isWalkable()).isPresent())
                    currentMoveDir = nextMoveDir;
                break;
            case STOP:
                currentMoveDir = nextMoveDir;
                break;
        }

        switch (currentMoveDir) {
            case UP:
                astar.moveToUpCell();
                break;
            case RIGHT:
                astar.moveToRightCell();
                break;
            case DOWN:
                astar.moveToDownCell();
                break;
            case LEFT:
                astar.moveToLeftCell();
                break;
            case STOP:
                astar.stopMovement();
                break;
        }
    }

    public void teleport() {
        astar.getGrid()
                .getRandomCell(AStarCell::isWalkable)
                .ifPresent(c -> astar.stopMovementAt(c.getX(), c.getY()));
    }
}
