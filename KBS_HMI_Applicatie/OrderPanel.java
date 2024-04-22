package KBS_HMI_Applicatie;

import javax.swing.*;
import javax.swing.border.Border;
import javax.swing.border.EmptyBorder;
import java.awt.*;

public class OrderPanel extends JPanel{
    private GUIThemes guiTheme;
    private Dimension screenDimension;
    private JButton manageOrderButton;
    private JButton startOrderButton;
    private JLabel orderLabel;
    private JLabel orders;
    public OrderPanel(Dimension screenDimension){
        this.screenDimension = screenDimension;
        Color darkGray = new Color(51, 51, 51);
        Color background = new Color(35, 35, 35);
        Color borderColor = new Color(58, 58, 58);
        Color foregroundColor = new Color(230, 230, 230);
        Color buttonColor = new Color(73, 73, 73);

        Border border = BorderFactory.createLineBorder(borderColor);
        Dimension buttonDimensions = new Dimension(170, 35);
        EmptyBorder panelBorderDimensions = new EmptyBorder(10, 10, 5, 10);
        Dimension buttonPanelDimensions = new Dimension(screenDimension.width/2-21, screenDimension.height);

        setBackground(darkGray);
        setLayout(new BorderLayout());
        setBorder(new EmptyBorder(0, 10, 10, 10));
        setPreferredSize(screenDimension);

        orders = new JLabel("Orders");
        orders.setForeground(foregroundColor);
        orders.setFont(new Font(orders.getFont().getName(), Font.PLAIN, 30));
        orders.setHorizontalAlignment(SwingConstants.CENTER);

        JPanel ordersPanel = new JPanel(new BorderLayout());
        ordersPanel.setBackground(background);
        ordersPanel.setBorder(border);
//        ordersPanel.setPreferredSize(new Dimension(screenDimension.width, 5));

        orderLabel = new JLabel("Order: none");
        orderLabel.setForeground(foregroundColor);
        orderLabel.setFont(new Font(orders.getFont().getName(), Font.PLAIN, 20));
        orderLabel.setHorizontalAlignment(SwingConstants.CENTER);

        JPanel buttonsPanel = new JPanel(new BorderLayout());
        buttonsPanel.setBackground(background);
        buttonsPanel.setBorder(panelBorderDimensions);
        buttonsPanel.setPreferredSize(new Dimension(screenDimension.width, 3));

        JPanel managebuttonPanel = new JPanel(new FlowLayout());
        managebuttonPanel.setBackground(background);
        managebuttonPanel.setBorder(panelBorderDimensions);
        managebuttonPanel.setPreferredSize(buttonPanelDimensions);

        JPanel startbuttonPanel = new JPanel(new FlowLayout());
        startbuttonPanel.setBackground(background);
        startbuttonPanel.setBorder(panelBorderDimensions);
        startbuttonPanel.setPreferredSize(buttonPanelDimensions);

        manageOrderButton = new JButton("Manage order");
        manageOrderButton.setBackground(buttonColor);
        manageOrderButton.setForeground(foregroundColor);
        manageOrderButton.setPreferredSize(buttonDimensions);

        startOrderButton = new JButton("Start order");
        startOrderButton.setBackground(buttonColor);
        startOrderButton.setForeground(foregroundColor);
        startOrderButton.setPreferredSize(buttonDimensions);

        add(orders, BorderLayout.NORTH);
        add(ordersPanel, BorderLayout.CENTER);
        ordersPanel.add(orderLabel, BorderLayout.NORTH);
        ordersPanel.add(buttonsPanel, BorderLayout.CENTER);

        buttonsPanel.add(managebuttonPanel, BorderLayout.WEST);
        managebuttonPanel.add(manageOrderButton);

        buttonsPanel.add(startbuttonPanel, BorderLayout.EAST);
        startbuttonPanel.add(startOrderButton);
    }
}
