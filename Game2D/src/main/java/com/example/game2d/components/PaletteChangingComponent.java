package com.example.game2d.components;

import com.almasb.fxgl.core.math.FXGLMath;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.texture.Texture;
import javafx.geometry.Rectangle2D;

public class PaletteChangingComponent extends Component {

    private Texture texture;

    private double lastX = 0;
    private double lastY = 0;

    private double timeToSwitch = 0;
    private int spriteColor = 0;

    public PaletteChangingComponent(Texture texture) {
        this.texture = texture;
    }

    @Override
    public void onAdded() {
        entity.getViewComponent().addChild(texture);
    }

    @Override
    public void onUpdate(double tpf) {
        timeToSwitch += tpf;

        if (timeToSwitch >= 5.0) {
            spriteColor = (int) (160 * FXGLMath.random(0, 2) * 0.24);
            timeToSwitch = 0;
        }

        double dx = entity.getX() - lastX;
        double dy = entity.getY() - lastY;

        lastX = entity.getX();
        lastY = entity.getY();

        if (dx == 0 && dy == 0) {
            // didn't move
            return;
        }

        if (Math.abs(dx) > Math.abs(dy)) {
            // move was horizontal
            if (dx > 0) {
                texture.setViewport(new Rectangle2D(130*3 * 0.24, spriteColor, 130 * 0.24, 160 * 0.24));
            } else {
                texture.setViewport(new Rectangle2D(130*2 * 0.24, spriteColor, 130 * 0.24, 160 * 0.24));
            }
        } else {
            // move was vertical
            if (dy > 0) {
                texture.setViewport(new Rectangle2D(0, spriteColor, 130 * 0.24, 160 * 0.24));
            } else {
                texture.setViewport(new Rectangle2D(130 * 0.24, spriteColor, 130 * 0.24, 160 * 0.24));
            }
        }
    }
}
