package KBS_HMI_Applicatie;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import java.awt.*;

public class StatusPanel extends JPanel{
    private GUIThemes guiTheme;
    private Dimension screenDimension;
    private JPanel statusPanel, logsPanel, statusLabelPanel, statusPanelLabel, statusLights;
    private JLabel statusLabel, status, stoplicht;
    private JLabel logs;
    private JTextField statussen;

    public StatusPanel(Dimension screenDimension){
        this.screenDimension = screenDimension;
        //De kleuren die in dit Paneel gebruikt worden. Deze worden met de integratie van Themas aangepast
        Color darkGray = new Color(51, 51, 51);
        Color background = new Color(35, 35, 35);
        Color borderColor = new Color(58, 58, 58);
        Color foregroundColor = new Color(204, 204, 204);
        //De borders die gebruikt worden
        EmptyBorder statussenBorder = new EmptyBorder(0,0,0,0);
        Border border = BorderFactory.createLineBorder(borderColor);

        //Past het status paneel aan
        setBackground(darkGray);
        setLayout(new BorderLayout());
        setBorder(new EmptyBorder(0, 10, 10, 10));
        setPreferredSize(screenDimension);

        //Het paneel bovenaan het statuspaneel wordt aangemaakt
        statusLabelPanel = new JPanel(new GridLayout(1, 3));
        statusLabelPanel.setBackground(darkGray);
        statusLabelPanel.setPreferredSize(new Dimension(screenDimension.width, 60));

        statusLights = new JPanel(new FlowLayout());
        statusLights.setBackground(darkGray);
        statusLights.setBorder(new EmptyBorder(20, 40, 0, 0));

        statusPanelLabel = new JPanel(new FlowLayout());
        statusPanelLabel.setBackground(darkGray);
        statusLabelPanel.setBorder(new EmptyBorder(5, 0, 0, 0));

        //Status kan geset worden op Stop, Manual of Automatic
        status = new JLabel("Manual");
        status.setForeground(foregroundColor);
        status.setFont(new Font(status.getFont().getName(), Font.PLAIN, 20));
        //Het lampje dat naast de status weergegeven wordt
        Icon yellowIcon = new ImageIcon("src/KBS_HMI_Applicatie/Icons/yellowCircle15.png");
        stoplicht = new JLabel(yellowIcon);

        //Het Label bovenaan het status paneel wordt aangemaakt
        statusLabel = new JLabel("Status");
        statusLabel.setForeground(foregroundColor);
        statusLabel.setFont(new Font(statusLabel.getFont().getName(), Font.PLAIN, 40));

        //Het logs paneel waar de logs in komen wordt aangemaakt
        logsPanel = new JPanel();
        logsPanel.setBackground(background);
        logsPanel.setBorder(border);

        //Het label bovenaan het logs paneel wordt aangemaakt
        logs = new JLabel("Logs:");
        logs.setForeground(foregroundColor);
        logs.setFont(new Font(logs.getFont().getName(), Font.PLAIN, 15));
        logs.setHorizontalAlignment(SwingConstants.CENTER);
        logs.setPreferredSize(new Dimension(300, logs.getFontMetrics(logs.getFont()).getHeight()));

        //Het statuspaneel die in het logs paneel geplaatst wordt wordt aangemaakt
        statusPanel = new JPanel();
        Dimension statusPanelDimension = new Dimension(300, 20 * statusPanel.getFontMetrics(statusPanel.getFont()).getHeight());
        statusPanel.setBackground(background);
        statusPanel.setBorder(border);
        statusPanel.setPreferredSize(statusPanelDimension);

        //Het textfield dat in het statuspaneel geplaatst wordt wordt aangemaakt
        statussen = new JTextField();
        Dimension statussenDimension = new Dimension(460, 19 * statussen.getFontMetrics(statussen.getFont()).getHeight());
        statussen.setForeground(Color.yellow);
        statussen.setBorder(statussenBorder);
        statussen.setBackground(background);
        statussen.setHorizontalAlignment(SwingConstants.LEFT);
        statussen.setPreferredSize(statussenDimension);

        //Alle onderdelen worden samengevoegd
        add(statusLabelPanel, BorderLayout.NORTH);
        statusLabelPanel.add(new JLabel(""));
        statusLabelPanel.add(statusPanelLabel);
        statusLabelPanel.add(statusLights);
        statusPanelLabel.add(statusLabel);
        statusLights.add(stoplicht);
        statusLights.add(status);

        add(logsPanel, BorderLayout.CENTER);
        logsPanel.add(logs, BorderLayout.CENTER);
        add(statusPanel, BorderLayout.SOUTH);
        statusPanel.add(statussen);
    }
}
