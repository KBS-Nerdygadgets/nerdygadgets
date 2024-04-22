package KBS_HMI_Applicatie;

import javax.swing.*;
import javax.swing.border.EmptyBorder;
import java.awt.*;
public class SerialConnectionManager extends JPanel{
    private GUIThemes guiTheme;
    private Dimension screenDimension;
    private JButton btn0, btn1, connectionButton, settingsButton;
    private JComboBox<String> startButton;
    public SerialConnectionManager(Dimension screenDimension){
        Color background = new Color(42, 42, 42);
        Color buttonColor = new Color(73, 73, 73);
        Color buttonForegroundColor = new Color(200, 200, 200);
        Color foregroundColor = new Color(230, 230, 230);

        Dimension buttonDimensions = new Dimension(125, 30);
        Dimension settingsButtonDimensions = new Dimension(30, 30);

        this.screenDimension = screenDimension;
        setBackground(background);
        setPreferredSize(screenDimension);
        setLayout(new BorderLayout());
        setBorder(new EmptyBorder(10, 10, 10, 10));

        JPanel leftPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        leftPanel.setBackground(background);

        JLabel select = new JLabel("Select Device:    ");
        select.setForeground(foregroundColor);
        select.setFont(new Font(select.getFont().getName(), Font.PLAIN, 20));

        String[] ports = {"//COM6", "//COM4"};
        startButton = new JComboBox<>(ports);
        startButton.setBackground(buttonColor);
        startButton.setForeground(buttonForegroundColor);
        startButton.setPreferredSize(buttonDimensions);
        startButton.setFont(new Font(startButton.getFont().getName(), Font.PLAIN, 20));

        connectionButton = new JButton("Connect");
        connectionButton.setBackground(buttonColor);
        connectionButton.setForeground(buttonForegroundColor);
        connectionButton.setPreferredSize(buttonDimensions);
        connectionButton.setFont(new Font(connectionButton.getFont().getName(), Font.PLAIN, 20));

        JPanel rightPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        rightPanel.setBackground(background);

        Icon settingsIcon = new ImageIcon("src/KBS_HMI_Applicatie/settingsIcon.png");
        settingsButton = new JButton(settingsIcon);
        settingsButton.setBackground(buttonColor);
        settingsButton.setPreferredSize(settingsButtonDimensions);

        leftPanel.add(select);
        leftPanel.add(startButton);
        leftPanel.add(connectionButton);

        rightPanel.add(settingsButton);

        add(leftPanel, BorderLayout.WEST);
        add(rightPanel, BorderLayout.EAST);
    }
}
