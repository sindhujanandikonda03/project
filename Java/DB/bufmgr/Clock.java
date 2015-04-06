/* Author: Xiaolong Cheng, Yu Yang
 * 
 */


package bufmgr;

import global.GlobalConst;
/*
 *  A clock algorithm for buffer pool replacement policy. It picks up the frame
 * in the buffer pool to be replaced.  
 * */
public class Clock implements GlobalConst {
    
	private int current;
	private final int numberOfFrame;
	private final FrameDesc[] frametab;
    private boolean availFrame;
	
	public Clock(BufMgr bufmgr) {
		current = 0;
		numberOfFrame = bufmgr.getNumFrames();
		frametab = bufmgr.frametab();
		availFrame = false;
	}
	
	/* 
	 * When the clock needs to pick a victim, it first considers the current frame.
     * If the current frame's state in invalid, the frame is chosen.
     * Otherwise, if the pin count is greater than 0, the frame is not chosen.
     * If the pin count is 0 and the reference bit is false, then the frame is chosen. 
     * If reference bit is true, set it to false and the next frame is considered.
     * 
     * */
	public int pickVictim() {
		
		for(int counter = 0; counter < numberOfFrame * 2; counter++) {

			if(frametab[current].getPageId().pid==INVALID_PAGEID)
			//if(frametab[current].getValid())
			{
				availFrame = true;
				break;
			}
			if(frametab[current].getPinCount() == 0) {
				if(frametab[current].getRef() == true) {
					frametab[current].setRef(false);
				}
				else{
					availFrame = true;
					break;
				}					
			}
			current = (current + 1) % numberOfFrame;

		} // End for loop
		
		if(availFrame == false)
			throw new IllegalStateException("No available Frame.");
		
		return current;   
	}
	
	

}
