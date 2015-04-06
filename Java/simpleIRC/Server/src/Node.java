
import java.net.*;
import java.io.*;

/**
 * every node represents a user
 */
public class Node {
	String username = null;
	String room = null;
	
	//every node(client) has its own socket and IO streams
	Socket socket = null;
	ObjectOutputStream output = null;
	ObjectInputStream input = null;
		
	Node next = null;
}