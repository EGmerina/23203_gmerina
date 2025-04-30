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
import static com.almasb.fxgl.dsl.FXGL.inc;
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

    public void up() {
        currentMoveDir = UP;
    }

    public void down() {
        currentMoveDir = DOWN;
    }

    public void left() {
        currentMoveDir = LEFT;
    }

    public void right() {
        currentMoveDir = RIGHT;
    }

    public void stop() {
        currentMoveDir = STOP;
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

        }
//        else {
//            if (texture.getAnimationChannel() != animIdle) {
//                texture.loopAnimationChannel(animIdle);
//            }
//        }

        switch (currentMoveDir) {
            case UP:
                astar.moveToUpCell();
                inc("score", 1);
                break;
            case RIGHT:
                astar.moveToRightCell();
                inc("score", 1);
                break;
            case DOWN:
                astar.moveToDownCell();
                inc("score", 1);
                break;
            case LEFT:
                astar.moveToLeftCell();
                inc("score", 1);
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
