package com.example.game2d;

import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.entity.component.Required;
import com.almasb.fxgl.pathfinding.CellMoveComponent;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;
import com.almasb.fxgl.texture.AnimatedTexture;
import com.almasb.fxgl.texture.AnimationChannel;
import javafx.geometry.Point2D;
import javafx.scene.image.Image;
import javafx.util.Duration;

import static com.almasb.fxgl.dsl.FXGL.image;
import static com.example.game2d.PlayerComponent.MoveDirection.*;


@Required(AStarMoveComponent.class)
public class PlayerComponent extends Component {

    private AnimatedTexture texture;

    private AnimationChannel animIdle, animWalkRight, animWalkLeft, animWalkUp, animWalkDown;

    public PlayerComponent() {

        Image image = image("Artyom.png");

        animIdle = new AnimationChannel(image, 24, 16, 32, Duration.seconds(1), 2, 2);
        animWalkRight = new AnimationChannel(image, 24, 16, 32, Duration.seconds(1), 0, 5);
        animWalkLeft = new AnimationChannel(image, 24, 16, 32, Duration.seconds(1), 12, 17);
        animWalkUp = new AnimationChannel(image, 24, 16, 32, Duration.seconds(1), 6, 11);
        animWalkDown = new AnimationChannel(image, 24, 16, 32, Duration.seconds(1), 18, 23);

        texture = new AnimatedTexture(animIdle);
        texture.loop();
    }

    enum MoveDirection {
        UP, RIGHT, DOWN, LEFT, STOP
    }

    private CellMoveComponent moveComponent;

    private AStarMoveComponent astar;

    private MoveDirection currentMoveDir = STOP;
    private MoveDirection nextMoveDir = STOP;

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


        if (astar.isMoving()) {

            if (texture.getAnimationChannel() != animWalkRight && currentMoveDir == RIGHT) {
                texture.loopAnimationChannel(animWalkRight);
            } else if (texture.getAnimationChannel() != animWalkLeft && currentMoveDir == LEFT) {
                texture.loopAnimationChannel(animWalkLeft);
            } else if (texture.getAnimationChannel() != animWalkUp && currentMoveDir == UP) {
                texture.loopAnimationChannel(animWalkUp);
            } else if (texture.getAnimationChannel() != animWalkDown && currentMoveDir == DOWN) {
                texture.loopAnimationChannel(animWalkDown);
            }

        } else {
            if (texture.getAnimationChannel() != animIdle) {
                texture.loopAnimationChannel(animIdle);
            }

        }

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

    @Override
    public void onAdded() {
        entity.getViewComponent().addChild(texture);

    }


}
