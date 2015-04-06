/* Author: Xiaolong Cheng, Yu Yang
 * 
 */


package bufmgr;

import global.GlobalConst;
import global.PageId;

/*
 *  Implement a frame descriptor class that includes the states such as pageId for 
 *  each page -- set INVALID_PAGEID to pid if that page is invalid, dirty bit to decide
 *  if a page is modified or not, the pin count, and changed pin count during the replacement
 *  algorithm. For example, if a page is pinned, the pin count should increase by 1. Otherwise,
 *  the pin count should decrease by 1 
 *  */

public class FrameDesc implements GlobalConst {
	
	private PageId pageno;
	private boolean dirty_bit;
	private int pin_count;
	private boolean refbit;
	private boolean valid_bit;
	
	/* Initialization */
	public FrameDesc() {
        pageno = new PageId();
        pageno.pid = INVALID_PAGEID;
        dirty_bit = false;
        pin_count = 0;
        refbit = true;
        valid_bit = false;
    }
	
	/* Return pageId of a frame */
	public PageId getPageId() {
		return pageno;
	}
	
	public void setPageId(PageId pid) {
		pageno= pid;
	}
	
	public void setPageIdInt(int pid) {
		pageno.pid= pid;
	}
	/* Get the current dirty status and check if the page is dirty or not */
	public boolean isDirty() {
		return dirty_bit;
	}
	
	/* Set dirty bit for a page */
	public void setDirty(boolean dirty) {
		dirty_bit = dirty;
	}
	
	/* Return the pin count */
	public int getPinCount() {
		return pin_count;
	}
	
	/* Increase by 1 if the frame is pinned */
	public int incrementPinCount() {
		return (++ pin_count);
	} 
	
	/* Decrease pin count by 1 if the frame is unpinned
	 *   if the pin count less or equal than 0, set the pin count to 0 */
	public int decrementPinCount() {
		
		pin_count = (pin_count > 0) ? pin_count - 1 : 0;

		return pin_count;		
	}
	
	/* Get the status of reference bit */
	public boolean getRef() {
		return refbit;
	}
	
	/* Set reference bit */
	public void setRef(boolean reference) {
		refbit = reference;
	}
	
	/* Get valid bit */
	public boolean getValid() {
		return valid_bit;
	}
	public void setValid(boolean valid){
		valid_bit = valid;
	}
}
