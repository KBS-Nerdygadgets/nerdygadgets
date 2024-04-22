package KBS_HMI_Applicatie;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import java.awt.*;

public class StatusPanel extends JPanel{
    private int marginTop;
    private int marginLeft;
    private int width;
    private int screenHeight;
    private int height;
    private GUIThemes guiTheme;
    private Dimension screenDimension;
    private OrderPanel orderPanel;
    private JLabel status;
    private JLabel logs;
    private JTextField statussen;

    public StatusPanel(Dimension screenDimension){
        Color darkGray1 = new Color(51, 51, 51);
        Color background = new Color(35, 35, 35);
        Color borderColor = new Color(58, 58, 58);
        Color foregroundColor = new Color(255, 255, 255);
        Border border = BorderFactory.createLineBorder(borderColor);

        this.screenDimension = screenDimension;
        setBackground(darkGray1);
        setLayout(new BorderLayout());
        setBorder(new EmptyBorder(10, 10, 10, 10));
        setPreferredSize(screenDimension);

        status = new JLabel("Status");
        status.setForeground(foregroundColor);
        status.setFont(new Font(status.getFont().getName(), Font.PLAIN, 40));
        status.setHorizontalAlignment(SwingConstants.CENTER);

        JPanel logsPanel = new JPanel();
        logsPanel.setBackground(background);
        logsPanel.setBorder(border);

        logs = new JLabel("Logs:");
        logs.setForeground(foregroundColor);
        logs.setFont(new Font(logs.getFont().getName(), Font.PLAIN, 13));
        logs.setHorizontalAlignment(SwingConstants.CENTER);
        logs.setPreferredSize(new Dimension(300, logs.getFontMetrics(logs.getFont()).getHeight()));

        JPanel statusPanel = new JPanel(new FlowLayout());
        Dimension statusPanelDimension = new Dimension(300, 13 * statusPanel.getFontMetrics(statusPanel.getFont()).getHeight());
        statusPanel.setBackground(background);
        statusPanel.setBorder(border);
        statusPanel.setPreferredSize(statusPanelDimension);

        statussen = new JTextField();
        Dimension statussenDimension = new Dimension(460, 12 * statussen.getFontMetrics(statussen.getFont()).getHeight());
        statussen.setForeground(Color.yellow);
        statussen.setOpaque(true);
        statussen.setBackground(background);
        statussen.setHorizontalAlignment(SwingConstants.LEFT);
        statussen.setPreferredSize(statussenDimension);

        add(status, BorderLayout.NORTH);
        add(logsPanel, BorderLayout.CENTER);
        logsPanel.add(logs, BorderLayout.CENTER);
        add(statusPanel, BorderLayout.SOUTH);
        statusPanel.add(statussen);
    }
}
