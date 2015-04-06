import javax.swing.*;
import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/*
 * message transfer class on server
 */
public class ServerReceive extends Thread {
	JTextArea textarea;
	JTextField textfield;
	JComboBox combobox;
	Node client;
	String rcv_name;
	String fileName;
	UserLinkList userLinkList;//users list
	//List<String> rooms = new ArrayList<String>();
	
	public boolean isStop;
	
	public ServerReceive(JTextArea textarea,JTextField textfield,
		JComboBox combobox,Node client,UserLinkList userLinkList){

		this.textarea = textarea;
		this.textfield = textfield;
		this.client = client;
		this.userLinkList = userLinkList;
		this.combobox = combobox;
		
		isStop = false;
		
	}
	
	public void run(){
		//send user list to all users
		sendUserList();
		boolean success = false;
		while(!isStop && !client.socket.isClosed()){
			try{
				String type = (String)client.input.readObject();
				Node node_nick;
				if(type.startsWith("Nick")){
					String []parts = type.split(" ");
				    node_nick =userLinkList.root;
					while(node_nick.next!=null){
						//String s = userLinkList.findUser(parts[1], room)
						if(node_nick.next.username.equalsIgnoreCase(parts[1])){
							if((userLinkList.findUser(parts[2], node_nick.next.room)) == null){
							    node_nick.next.username = parts[2];
							    success = true;
							    break;
							}
							else{
								node_nick.next.output.writeObject("NAME_COLLISION");
							}
						}
						node_nick=node_nick.next;
					}
					//sendUserList_Room(node_nick.room); 
					if(success == true){
					sendUserList();
				    sendToGroup(parts[1] + " changes nickname to " + parts[2], node_nick.next.room);
				    
				    int i = 0;
				    while( i <= combobox.getItemCount()){
				    	String item = combobox.getItemAt(i).toString();
				    	System.out.println(item);
				    	if(item.equalsIgnoreCase(parts[1])){
				    		combobox.removeItemAt(i);
				    		combobox.addItem(parts[2]);
				    		break;
				    	}
				    	i++;
				    }
					}
				}
				
				if(type.startsWith("SEND_FILE")){
					
					String parse[] = type.split(" ");
					System.out.println("rcv:" + parse[1]);
					rcv_name = parse[1];
					fileName = parse[2];
					//Node node=userLinkList.findUser(parse[1]);
					
					 try {
				            //node.output.writeObject("SEND_FILE " + parse[2]);
				            
						    FileOutputStream outStream = new FileOutputStream("E:/IRC-master/IRC-master/"+ fileName);
				            byte[] buffer = new byte[2000];
				            int bytesRead = 0, counter = 0;
				 
				            while ((bytesRead = client.input.read(buffer)) > 0) {
				
				                //System.out.println("bytes read:"+bytesRead);
				               
				                    outStream.write(buffer, 0, bytesRead);
				                    //node.output.write(buffer, 0, bytesRead);
				                    counter += bytesRead;
				                    //System.out.println("total bytes read: " + counter);
				                
				              //  if (bytesRead < 1024) {
				                //    outStream.flush();
				                //    break;
				              //  }
				            }
				            
				            outStream.close();
				 
				        } catch (Exception e) {
				        	e.printStackTrace();
				        }

				}
				if(type.equalsIgnoreCase("SEND_COMPLETE")){
					System.out.print(fileName);
					File file = new File("E:/IRC-master/IRC-master/"+ fileName);
					TransferFile(file, rcv_name);
					
				}
				/* --if the received is chat message-- */
				if(type.equalsIgnoreCase("chatMsg")){
					
					//read the subsequent objects if it's a chatMsg...
					String toSomebody = (String)client.input.readObject();
					String status  = (String)client.input.readObject();
					String action  = (String)client.input.readObject();
					String message = (String)client.input.readObject();
					
					String msg = client.username 
							+" "+ action
							+ " says to "
							+ toSomebody 
							+ "  : "
							+ message
							+ "\n";
					if(status.equalsIgnoreCase("private")){
						msg = " [private] " + msg;
					}
					
					textarea.append(msg);
					
					//if 'all', forward to all user
					if(toSomebody.equalsIgnoreCase("all")){
						sendToGroup(msg, client.room);//send to all
						//sendToAll(msg);
					}
					//else forward to only one specific user
					else{
						try{
						//show message in the sender's own window
							client.output.writeObject("chatMsg");
							client.output.flush();
							client.output.writeObject(msg);
							client.output.flush();
						}
						catch (Exception e){
							//System.out.println("###"+e);
						}
						
						//write message to the receiver's stream
						Node node = userLinkList.findUser(toSomebody);						
						if(node != null){
							node.output.writeObject("chatMsg"); 
							node.output.flush();
							node.output.writeObject(msg);
							node.output.flush();
						}
					}
				}
				
				/* --if the received is a leaving notice -- */
				else if(type.equalsIgnoreCase("userLeft")){
					userLinkList.delUser(client.username);
					
					String msg = "user " + client.username + " has left\n";

					combobox.removeAllItems();
					combobox.addItem("all");
					
					//load updated list into combobox
					Node n= userLinkList.root;
					while(n.next != null){
						combobox.addItem(n.next.username);
						n=n.next;
					}					
					combobox.setSelectedIndex(0);
					textarea.append(msg);
					textfield.setText("active user:" + userLinkList.getCount() + "users\n");
					
					sendToAll(msg);//send message to all
					sendUserList();//re-send user list to update
					
					break;
				}
			}
			catch (Exception e){
				//System.out.println(e);
			}
		}
	}
	
	/*
	 * send message to all active users 
	 */
	public void sendToAll(String msg){
		//write to users' stream one by one...
		Node node=userLinkList.root;
		while(node.next!=null){
			System.out.println(node.next.username);
			try{
				node.next.output.writeObject("chatMsg");
				node.next.output.flush();
				node.next.output.writeObject(msg);
				node.next.output.flush();
			}
			catch (Exception e){
				//System.out.println(e);
			}
			node= node.next;
		}//end-while
		
	}
   //send msg to all users in the same group
   public void sendToGroup(String msg, String room){
	   Node node=userLinkList.root;
	   while(node.next!=null){
			try{
				if(node.next.room.equalsIgnoreCase(room)){
				   node.next.output.writeObject("chatMsg");
				   node.next.output.flush();
				   node.next.output.writeObject(msg);
				   node.next.output.flush();
			    }
			}
			catch (Exception e){
				//System.out.println(e);
			}
			node= node.next;
		}//end-while
   }
 
	/*
	 * send updated user list to all active users
	 */
   public void sendUserList(){
		//package all the usernames into a long string		
		String userlist = "";	
		String channel_list = "";
		
		Node iter=userLinkList.root;
		
		while(iter.next!=null){
			
			userlist+=iter.next.room + ' ' + iter.next.username + '\n';
			
			if(channel_list.indexOf(iter.next.room) == -1) {
					channel_list+=iter.next.room +'\n';
					System.out.println(iter.next.room);
				  }
			iter=iter.next;
		}

		//send the list to all the users one by one..
		Node node=userLinkList.root;
		while(node.next!=null){
			try{
				node.next.output.writeObject("userList");
				node.next.output.flush();
				node.next.output.writeObject(userlist+"#"+channel_list+"#"+node.next.room);
				node.next.output.flush();
			} catch (Exception e){}			
			node=node.next;
		}//endwhile
		
	}

   public void TransferFile(File file, String user){
		long fileSize = file.length();
		int smallSize = (int) fileSize;
		long completed = 0;
		int step = 1500, left;
		byte[] buffer;
		
		Node node=userLinkList.findUser(user);
		
		try{
		   FileInputStream fileStream = new FileInputStream(file);
		   node.output.writeObject("SEND_FILE" + " " + file.getName() + " " + fileSize);		   
	
		   buffer = new byte[step];
	
	       while (completed <= fileSize) {
	    	   left = (int) (fileSize - completed);
	    	   fileStream.read(buffer);
	    	   if( left <= step)
	    		   node.output.write(buffer, 0, left);
	    	   else
	               node.output.write(buffer);
	           completed += step;
	        }
	       node.output.writeObject("SEND_COMPLETE");
	       node.output.flush();
	       fileStream.close();


		}catch(Exception e){ e.printStackTrace(); }
	}
}