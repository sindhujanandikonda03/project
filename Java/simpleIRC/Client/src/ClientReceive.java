
import javax.swing.*;
import java.io.*;
import java.net.*;

/*
 * This class is an embedded Thread of the client program 
 * it takes care of receiving message from the server and update
 * the interface
 */
public class ClientReceive extends Thread {
	private JComboBox combobox;
	private JComboBox channel_box;
	private JTextArea textarea;
	
	Socket socket;
	ObjectOutputStream output;
	ObjectInputStream  input;
	JTextField showStatus;
	boolean conn;

	public ClientReceive(Socket socket,ObjectOutputStream output,
		ObjectInputStream  input,JComboBox combobox,JComboBox channel_box,JTextArea textarea,JTextField showStatus){

		this.socket = socket;
		this.output = output;
		this.input = input;
		this.combobox = combobox;
		this.channel_box = channel_box;
		this.textarea = textarea;
		this.showStatus = showStatus;
	}
	
	public void run(){
		while(!socket.isClosed()){
			try{
				String type = (String)input.readObject();
				String FileReceivedName = null;
				
				if(type.equalsIgnoreCase("systemMsg")){
					//if it's a msg, next object should be the message contained
					String sysmsg = (String)input.readObject();
					textarea.append("system msg: "+sysmsg);
				}
				else if(type.equalsIgnoreCase("serviceClose")){
					output.close();
					input.close();
					socket.close();					
					textarea.append("server is down!\n");				
					break;
				}
				else if(type.equalsIgnoreCase("chatMsg")){
					//chat message in next object
					String message = (String)input.readObject();
					textarea.append(message);
				}
				else if(type.equalsIgnoreCase("userList")){
					//updated user list
					String list = (String)input.readObject();
					//System.out.print(list);
					String user_ch[] = list.split("#");
					int active_users = 0;
					//System.out.print(user_ch[1]);
					
					String username_room[] = user_ch[0].split("\n");
					String channel_list[] = user_ch[1].split("\n");
					String user_room = user_ch[2];
					
					combobox.removeAllItems();
					channel_box.removeAllItems();
					
					System.out.print(username_room[0]);
					int i =0, j=0;
					combobox.addItem("all");
					while(i < username_room.length){
						String usernames[] = username_room[i].split(" ");
						if(usernames[0].equalsIgnoreCase(user_room)){
						    combobox.addItem(usernames[1]);
						    active_users++;
						}
						    i ++;
					}
					while(j < channel_list.length){
						channel_box.addItem(channel_list[j]);
						j ++;
					}
					combobox.setSelectedIndex(0);
					showStatus.setText("active users: " + active_users + " users");
				}
				else if(type.startsWith("SEND_FILE")){
					textarea.append("receiving file....\n");
					String parse[] = type.split(" ");
				    FileReceivedName = parse[1];
					try{
						FileOutputStream outStream = new FileOutputStream("E:/IRC-master/fileRCV" + FileReceivedName);
						byte[] buffer = new byte[2000];
			            int bytesRead = 0, counter = 0;
			            
			            while ((bytesRead = input.read(buffer)) > 0) {
							
			                System.out.println("bytes read:"+bytesRead);
			               
			                    outStream.write(buffer, 0, bytesRead);
			                   
			                    counter += bytesRead;
			                    System.out.println("total bytes read: " + counter);
			                
			              //  if (bytesRead < 1024) {
			                //    outStream.flush();
			                //    break;
			              //  }
			            }
			            
			            outStream.close();
					}
					catch(Exception e){ e.printStackTrace(); }
				}
				else if(type.startsWith("SEND_COMPLETE")){
					textarea.append("Done! File received successfully....\n");
				}
				else if(type.equalsIgnoreCase("ERROR")) {
					//conn = true;
					textarea.append("User exists.\n");
				}
				else if(type.equalsIgnoreCase("NAME_COLLISION")) {
					textarea.append("Nickname exits. Please change to another.\n");
				}
				else if(type.equalsIgnoreCase("LOGIN_SUCCESS")){
					conn = true;
				}
			}
			catch (Exception e ){
				System.out.println(e);
			}
		}
	}
}