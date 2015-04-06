
import java.awt.*;
import javax.swing.border.*;
import java.net.*;
import javax.swing.*;
import java.awt.event.*;

/**
 * class for generation the user config window
 * users specify there information and identity here
 */
public class UserConf extends JDialog {
	JPanel panelUserConf = new JPanel();
	
	JButton save = new JButton();
	JButton cancel = new JButton();
	JLabel DLGINFO=new JLabel("     default user:yyang");

	JPanel panelSave = new JPanel();
	JLabel message = new JLabel();
	JLabel room_t = new JLabel();
	
	String userInputName;
	String topic;
	JTextField userName ;
	JTextField roomTopic;
	
	public UserConf(JFrame frame,String str, String room) {
		super(frame, true);
		this.userInputName = str;
		this.topic = room;
		try {
			jbInit();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		//make the dialogue box centered
		Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		this.setLocation( (int) (screenSize.width - 400) / 2 + 50,
						(int) (screenSize.height - 600) / 2 + 150);
		this.setResizable(false);
	}

	private void jbInit() throws Exception {
		this.setSize(new Dimension(300, 160));
		this.setTitle("user identity config");
		message.setText("username:");
		room_t.setText("Topic:");
		
		userName = new JTextField(10);
		userName.setText(userInputName);
		
		roomTopic = new JTextField(10);
		roomTopic.setText(topic);
		
		save.setText("save");
		cancel.setText("cancel");

		panelUserConf.setLayout(new GridLayout(2,2));

		panelUserConf.add(message);
		panelUserConf.add(userName);
		panelUserConf.add(room_t);
		panelUserConf.add(roomTopic);

		panelSave.add(new Label("              "));
		panelSave.add(save);
		panelSave.add(cancel);
		panelSave.add(new Label("              "));

		Container contentPane = getContentPane();
		contentPane.setLayout(new BorderLayout());
		contentPane.add(panelUserConf, BorderLayout.NORTH);
		contentPane.add(DLGINFO, BorderLayout.CENTER);
		contentPane.add(panelSave, BorderLayout.SOUTH);

		//event handling for save button
		save.addActionListener(
			new ActionListener() {
				public void actionPerformed (ActionEvent a) {
					if(userName.getText().equals("")){
						DLGINFO.setText(" username cant be empty!");
						userName.setText(userInputName);
						return;
					}
					else if(userName.getText().length() > 15){
						DLGINFO.setText(" username length cant exceed 15!");
						userName.setText(userInputName);
						return;
					}
					userInputName = userName.getText();
					topic = roomTopic.getText();
					dispose();
				}
			}
		);

		//event handler for dialogue box closing
		this.addWindowListener(
			new WindowAdapter(){
				public void windowClosing(WindowEvent e){
				}
			}
		);

		//event handler for cancel
		cancel.addActionListener(
			new ActionListener(){
				public void actionPerformed(ActionEvent e){
					dispose();
				}
			}
		);
	}
}