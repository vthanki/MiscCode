package com.vishal.learning;

import java.util.ArrayList;

/**
 * Created by vishal.thanki@teufel.local on 7/28/16.
 */
public class Polygon {
    ArrayList<Side> sides;

    Polygon() {
        sides = new ArrayList<>();
        sides.add(new Side(10));
        sides.add(new Side(10));
        sides.add(new Side(10));
        sides.add(new Side(10));
    }

    int getPerimeter() {
        int result = 0;

        for (Side side:sides) {
            result += side.getLength();
        }
        return result;
    }

    void addSide(Side side) {
        sides.add(side);
    }
}
