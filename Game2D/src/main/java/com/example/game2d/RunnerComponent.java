package com.example.game2d;

import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.pathfinding.CellMoveComponent;
import com.almasb.fxgl.pathfinding.astar.AStarMoveComponent;
import com.almasb.fxgl.texture.AnimatedTexture;
import com.almasb.fxgl.texture.AnimationChannel;
import javafx.geometry.Point2D;
import javafx.scene.image.Image;
import javafx.util.Duration;

import java.util.List;

import static com.almasb.fxgl.dsl.FXGL.*;
import static com.example.game2d.PlayerComponent.MoveDirection.*;

public class RunnerComponent extends Component {
    private AStarMoveComponent astar;
    private CellMoveComponent moveComponent;
    private int PLAYER_SPACE_RADIOUS = 32 * 32;
    private int MAX_SPACE_FROM_PLAYER = 32 * 32 * 120;

    private AnimatedTexture texture;

    private AnimationChannel animIdle, animWalkRight, animWalkLeft, animWalkUp, animWalkDown;

    private PlayerComponent.MoveDirection currentMoveDir = STOP;

    private final List<Image> runnerImages = List.of(
            image("runner1.png"),
            image("runner.png"),
            image("runner2.png")
    );

    public RunnerComponent() {

        Image image = runnerImages.get(FXGL.random(0, runnerImages.size() - 1));

       // Image image = image("runner2.png");

        animIdle = new AnimationChannel(image, 24, 16, 32, Duration.seconds(1), 2, 2);
        animWalkRight = new AnimationChannel(image, 24, 16, 32, Duration.seconds(1), 0, 5);
        animWalkLeft = new AnimationChannel(image, 24, 16, 32, Duration.seconds(1), 12, 17);
        animWalkUp = new AnimationChannel(image, 24, 16, 32, Duration.seconds(1), 6, 11);
        animWalkDown = new AnimationChannel(image, 24, 16, 32, Duration.seconds(1), 18, 23);

        texture = new AnimatedTexture(animIdle);
        texture.loop();
    }

    @Override
    public void onUpdate(double tpf) {

        catchUp();

        currentMoveDir = getCurrentMovDir();

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
    }

    private PlayerComponent.MoveDirection getCurrentMovDir() {
        var player = FXGL.getGameWorld().getSingleton(GameType.PLAYER);

        int playerX = (int) player.getX();
        int playerY = (int) player.getY();

        var myX = entity.getX();
        var myY = entity.getY();

        if (playerX - myX > 0) {
            return RIGHT;
        } else if (playerX - myX < 0) {
            return LEFT;
        } else if (playerY - myY > 0) {
            return DOWN;
        } else if (playerY - myY < 0) {
            return UP;
        }
        return STOP;
    }

    private void catchUp() {
        var player = FXGL.getGameWorld().getSingleton(GameType.PLAYER);

        int playerX = (int) player.getX();
        int playerY = (int) player.getY();

        int playerCellX = player.call("getCellX");
        int playerCellY = player.call("getCellY");

        var myX = entity.getX();
        var myY = entity.getY();


        if (Math.pow(myX - playerX, 2) + Math.pow(myY - playerY, 2) < PLAYER_SPACE_RADIOUS) {
            astar.stopMovement();
            return;
        } else if (Math.pow(myX - playerX, 2) + Math.pow(myY - playerY, 2) > MAX_SPACE_FROM_PLAYER) {
            FXGL.getGameWorld().removeEntity(entity);
            inc("lostRunners", 1);
            return;
        }
        astar.moveToCell(playerCellX, playerCellY);

    }

    @Override
    public void onAdded() {
        entity.getViewComponent().addChild(texture);
    }
}

