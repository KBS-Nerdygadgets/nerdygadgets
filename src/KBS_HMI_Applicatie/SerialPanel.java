package KBS_HMI_Applicatie;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import java.awt.*;
public class SerialPanel extends JPanel{
    private GUIThemes guiTheme;
    private Dimension screenDimension;
    private JLabel rX, tX, rood, groen;
    private JButton connectionButton, settingsButton;
    private JComboBox<String> startButton;
    private JPanel serialStatus;
    public SerialPanel(Dimension screenDimension){
        this.screenDimension = screenDimension;
        //De kleuren die in dit Paneel gebruikt worden. Deze worden met de integratie van Themas aangepast
        Color background = new Color(42, 42, 42);
        Color buttonColor = new Color(73, 73, 73);
        Color buttonForegroundColor = new Color(200, 200, 200);
        Color foregroundColor = new Color(204, 204, 204);
        Color gridForeground = new Color(78, 78, 78);
        //De dimensions en borders die gebruikt worden
        Dimension buttonDimensions = new Dimension(125, 30);
        Dimension settingsButtonDimensions = new Dimension(30, 30);
        Border gridBorder = BorderFactory.createLineBorder(gridForeground);

        //Past het serial connection manager paneel aan
        setBackground(background);
        setPreferredSize(screenDimension);
        setLayout(new BorderLayout());
        setBorder(new EmptyBorder(10, 10, 10, 10));

        //Het linker paneel wordt aangemaakt, bevat de select device label, combobox en connect knop
        JPanel leftPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        leftPanel.setBackground(background);

        //Select device label wordt aangemaakt
        JLabel select = new JLabel("Select device:    ");
        select.setForeground(foregroundColor);
        select.setFont(new Font(select.getFont().getName(), Font.PLAIN, 20));

        //Combobox bevat alle com poorten
        String[] ports = {"//COM6", "//COM4"};
        startButton = new JComboBox<>(ports);
        startButton.setBackground(buttonColor);
        startButton.setForeground(buttonForegroundColor);
        startButton.setPreferredSize(buttonDimensions);
        startButton.setFont(new Font(startButton.getFont().getName(), Font.PLAIN, 20));

        //Connect knop wordt aangemaakt
        connectionButton = new JButton("Connect");
        connectionButton.setBackground(buttonColor);
        connectionButton.setForeground(buttonForegroundColor);
        connectionButton.setPreferredSize(buttonDimensions);
        connectionButton.setFont(new Font(connectionButton.getFont().getName(), Font.PLAIN, 20));

        //Om de status van de seriele communicatie aan te geven maken nog een paneel
        serialStatus = new JPanel(new GridLayout(2, 2));
        serialStatus.setBackground(background);
        serialStatus.setPreferredSize(new Dimension(80, 40));

        //De labels en gekleurde rondjes voor het communicatie paneel worden aangemaakt
        rX = new JLabel("RX");
        rX.setForeground(foregroundColor);
        rX.setHorizontalAlignment(SwingConstants.CENTER);
        tX = new JLabel("TX");
        tX.setForeground(foregroundColor);
        tX.setHorizontalAlignment(SwingConstants.CENTER);
        Icon redIcon = new ImageIcon("src/KBS_HMI_Applicatie/Icons/redCircle.png");
        rood = new JLabel(redIcon);
        Icon greenIcon = new ImageIcon("src/KBS_HMI_Applicatie/Icons/greenCircle.png");
        groen = new JLabel(greenIcon);

        //Rechter paneel wordt aangemaakt, bevat de settings knop
        JPanel rightPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        rightPanel.setBackground(background);

        //De settings knop wordt aangemaakt
        Icon settingsIcon = new ImageIcon("src/KBS_HMI_Applicatie/Icons/settingsIcon.png");
        settingsButton = new JButton(settingsIcon);
        settingsButton.setBackground(buttonColor);
        settingsButton.setPreferredSize(settingsButtonDimensions);

        //Alle panelen worden aangevuld
        leftPanel.add(select);
        leftPanel.add(startButton);
        leftPanel.add(connectionButton);
        serialStatus.add(rX);
        serialStatus.add(tX);
        serialStatus.add(rood);
        serialStatus.add(groen);
        leftPanel.add(serialStatus);

        rightPanel.add(settingsButton);

        add(leftPanel, BorderLayout.WEST);
        add(rightPanel, BorderLayout.EAST);
    }
}
