package KBS_HMI_Applicatie;

import java.awt.*;

public class GUIThemes implements ITheme{
    public enum Theme{
        DarkTheme,
        LightTheme
    }
    private Theme theme;

    Color darkGray = new Color(42, 42, 42);
}
