package com.example.game2d;

import com.almasb.fxgl.animation.Interpolators;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.input.UserAction;
import com.almasb.fxgl.physics.PhysicsComponent;
import com.almasb.fxgl.scene.SubScene;
import com.almasb.fxgl.texture.AnimatedTexture;
import com.almasb.fxgl.texture.AnimationChannel;
import com.almasb.fxgl.texture.Texture;
import com.almasb.fxgl.ui.FontFactory;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.geometry.Insets;
import javafx.geometry.Point2D;
import javafx.geometry.Pos;
import javafx.scene.effect.DropShadow;
import javafx.scene.image.Image;
import javafx.scene.input.MouseButton;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.StackPane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.scene.text.Text;
import javafx.util.Duration;

import static com.almasb.fxgl.dsl.FXGL.*;

public class LevelEndScene extends SubScene {

    private static final int WIDTH = 400;
    private static final int HEIGHT = 250;

    private Text textUserTime = getUIFactoryService().
            newText("", Color.WHITE, 24.0);
    private HBox gradeBox = new HBox();

    private BooleanProperty isAnimationDone = new
            SimpleBooleanProperty(false);

    public LevelEndScene() {
        var bg = new Rectangle(WIDTH, HEIGHT, Color.color(0, 0, 0, 0.85));
        bg.setStroke(Color.BLUE);
        bg.setStrokeWidth(1.75);
        bg.setEffect(new DropShadow(28, Color.color(0, 0, 0, 0.9)));

        VBox.setVgrow(gradeBox, Priority.ALWAYS);

        var textContinue = getUIFactoryService().newText("tap to continue", Color.WHITE, 11.0);
        textContinue.visibleProperty().bind(isAnimationDone);

        animationBuilder()
                .repeatInfinitely()
                .autoReverse(true)
                .scale(textContinue)
                .from(new Point2D(1.25, 1.25))
                .buildAndPlay();

        var vbox = new VBox(15, textUserTime, gradeBox, textContinue);
        vbox.setAlignment(Pos.CENTER);
        vbox.setPadding(new Insets(25));

        var root = new StackPane(bg, vbox);

        root.setTranslateX(1280 / 2 - WIDTH / 2);
        root.setTranslateY(720 / 2 - HEIGHT / 2);
        getContentRoot().getChildren().addAll(root);
        getInput().addAction(new UserAction("Close Level End Screen") {
            @Override
            protected void onActionBegin() {
                if (!isAnimationDone.getValue())
                    return;
                getSceneService().popSubScene();
            }
        }, MouseButton.PRIMARY);


    }

    public void onLevelFinish() {
        isAnimationDone.setValue(false);
        Duration userTime = Duration.seconds(getd("levelTime"));

        for (int i = 0; i < gradeBox.getChildren().
                size(); i++) {
            var builder = animationBuilder(this).
                    delay(Duration.seconds(i * 0.75))
                    .duration(Duration.seconds(0.75))
                    .interpolator(Interpolators.ELASTIC.
                            EASE_OUT());
            // if last star animation
            if (i == gradeBox.getChildren().size() - 1) {
                builder = builder.onFinished(() ->
                        isAnimationDone.setValue(true));
            }
            builder.translate(gradeBox.getChildren().get(i))
                    .from(new Point2D(0, -500))
                    .to(new Point2D(0, 0))
                    .buildAndPlay();
        }
        getSceneService().pushSubScene(this);
    }

    public class PlayerComponent extends Component {
        private PhysicsComponent physics;
        private AnimatedTexture texture;
        private AnimationChannel animIdle;
        private AnimationChannel animWalk;

        public PlayerComponent() {
            Image image = image("player.png");
            animIdle = new AnimationChannel(image, 4, 32, 42,
                    Duration.seconds(1), 1, 1);
            animWalk = new AnimationChannel(image, 4, 32, 42,
                    Duration.seconds(0.66), 0, 3);
            texture = new AnimatedTexture(animIdle);
            texture.loop();
        }

        @Override
        public void onAdded() {
            entity.getTransformComponent().setScaleOrigin(new
                    Point2D(16, 21));
            entity.getViewComponent().addChild(texture);

        }

        @Override
        public void onUpdate(double tpf) {
            if (physics.isMovingX()) {
                if (texture.getAnimationChannel() != animWalk) {
                    texture.loopAnimationChannel(animWalk);
                }
            } else {
                if (texture.getAnimationChannel() != animIdle) {
                    texture.loopAnimationChannel(animIdle);
                }
            }
        }
    }
}
