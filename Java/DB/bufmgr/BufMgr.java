/* Author: Xiaolong Cheng, Yu Yang
 * 
 */
package bufmgr;

import global.GlobalConst;
import global.Minibase;
import global.Page;
import global.PageId;


import java.util.HashMap;

/**
 * <h3>Minibase Buffer Manager</h3>
 * The buffer manager manages an array of main memory pages.  The array is
 * called the buffer pool, each page is called a frame.  
 * It provides the following services:
 * <ol>
 * <li>Pinning and unpinning disk pages to/from frames
 * <li>Allocating and deallocating runs of disk pages and coordinating this with
 * the buffer pool
 * <li>Flushing pages from the buffer pool
 * <li>Getting relevant data
 * </ol>
 * The buffer manager is used by access methods, heap files, and
 * relational operators.
 */
public class BufMgr implements GlobalConst {
	  
	//buffer pool, an array of Page objects
	  Page BufPool[]; 
	  
	// frame descriptor, an array of FrameDesc objects
	  FrameDesc frametab[]; 
	  
	//map PageId with an integer, which is the index of BufPool[] and frametab[]
	  HashMap<Integer, Integer> map = new HashMap<Integer, Integer>();
	  
	  int total_numframes;
	  int unpinned_numframes;
	  
	  Clock replPolicy ;
	  
  /**
   * Constructs a buffer manager by initializing member data.  
   * 
   * @param numframes number of frames in the buffer pool
   */
  public BufMgr(int numframes) {	  
	  
	  BufPool= new Page[numframes]; //initialize the buffer pool
	  
      frametab = new FrameDesc[numframes];
      
      total_numframes= numframes;

      for(int i = 0; i < total_numframes; i++) {
 	  
    	  BufPool[i] = new Page();
    	  frametab[i] = new FrameDesc();          
      }
      
      replPolicy = new Clock(this);
	  
  } // public BufMgr(int numframes)

  /**
   * The result of this call is that disk page number pageno should reside in
   * a frame in the buffer pool and have an additional pin assigned to it, 
   * and mempage should refer to the contents of that frame. <br><br>
   * 
   * If disk page pageno is already in the buffer pool, this simply increments 
   * the pin count.  Otherwise, this<br> 
   * <pre>
   * 	uses the replacement policy to select a frame to replace
   * 	writes the frame's contents to disk if valid and dirty
   * 	if (contents == PIN_DISKIO)
   * 		read disk page pageno into chosen frame
   * 	else (contents == PIN_MEMCPY)
   * 		copy mempage into chosen frame
   * 	[omitted from the above is maintenance of the frame table and hash map]
   * </pre>		
   * @param pageno identifies the page to pin
   * @param mempage An output parameter referring to the chosen frame.  If
   * contents==PIN_MEMCPY it is also an input parameter which is copied into
   * the chosen frame, see the contents parameter. 
   * @param contents Describes how the contents of the frame are determined.<br>  
   * If PIN_DISKIO, read the page from disk into the frame.<br>  
   * If PIN_MEMCPY, copy mempage into the frame.<br>  
   * If PIN_NOOP, copy nothing into the frame - the frame contents are irrelevant.<br>
   * Note: In the cases of PIN_MEMCPY and PIN_NOOP, disk I/O is avoided.
   * @throws IllegalArgumentException if PIN_MEMCPY and the page is pinned.
   * @throws IllegalStateException if all pages are pinned (i.e. pool is full)
   */
  public void pinPage(PageId pageno, Page mempage, int contents) {

	  if (map.containsKey(new Integer(pageno.pid))){//page in the pool
		 
		  int idx = map.get(new Integer(pageno.pid)).intValue();
		//  if (contents==PIN_MEMCPY && frametab[idx].getPinCount()!=0){
		//	  throw new IllegalArgumentException("page is pinned!");
	//	  } 
		  frametab[idx].incrementPinCount();
		  return;
	  }
	  else {       /*page not in pool*/
		  if (getNumUnpinned()==0){
			  throw new IllegalStateException("pool is full");
		  }
		  
		  int idx = replPolicy.pickVictim();
		  if(frametab[idx].getValid() && frametab[idx].isDirty()){
			  flushPage(frametab[idx].getPageId());
		  }
		  map.remove(new Integer(frametab[idx].getPageId().pid));
		  
		  
		  if (contents==PIN_DISKIO){
			  Minibase.DiskManager.read_page(pageno, BufPool[idx]);
			  mempage.setPage(BufPool[idx]);
		  }
		  else if (contents==PIN_MEMCPY){
			  if ( frametab[idx].getPinCount()!=0){
				  throw new IllegalArgumentException("page is pinned!");
			  } 
			  BufPool[idx].copyPage(mempage);
			  mempage.setPage(BufPool[idx]);
		  }
		  else if (contents==PIN_NOOP){
			  mempage.setPage(BufPool[idx]);
		  }
		  frametab[idx].setPageId(new PageId(pageno.pid));
		  frametab[idx].setDirty(false);
		  frametab[idx].setValid(true);
		  frametab[idx].setRef(true);
		  frametab[idx].incrementPinCount(); 
		  map.put(new Integer(pageno.pid), new Integer(idx));
		  
	  }

	  
  } // public void pinPage(PageId pageno, Page page, int contents)
  
  /**
   * Unpins a disk page from the buffer pool, decreasing its pin count.
   * 
   * @param pageno identifies the page to unpin
   * @param dirty UNPIN_DIRTY if the page was modified, UNPIN_CLEAN otherwise
   * @throws IllegalArgumentException if the page is not in the buffer pool/.
   *  or not pinned
   */
  public void unpinPage(PageId pageno, boolean dirty) {

	  if (!map.containsKey(new Integer(pageno.pid))) //if page not in pool
		  throw new IllegalArgumentException("page not in the buffer pool");
	  else {
		  int idx = map.get(new Integer(pageno.pid)).intValue(); //get the frame index
		  if (frametab[idx].getPinCount()==0 ) {
		    throw new IllegalArgumentException("page not in the buffer pool");
		  }
		 
	      if (dirty == true){ frametab[idx].setDirty(dirty);}
		  frametab[idx].decrementPinCount();		
  }		  
 } // public void unpinPage(PageId pageno, boolean dirty)
  
  /**
   * Allocates a run of new disk pages and pins the first one in the buffer pool.
   * The pin will be made using PIN_MEMCPY.  Watch out for disk page leaks.
   * 
   * @param firstpg input and output: holds the contents of the first allocated page
   * and refers to the frame where it resides
   * @param run_size input: number of pages to allocate
   * @return page id of the first allocated page
   * @throws IllegalArgumentException if firstpg is already pinned
   * @throws IllegalStateException if all pages are pinned (i.e. pool exceeded)
   */
  public PageId newPage(Page firstpg, int run_size) {

	  PageId pid = new PageId();  
	  pid = Minibase.DiskManager.allocate_page(run_size);
	  pinPage(pid, firstpg, PIN_MEMCPY); 
	  return pid;
	  
  } // public PageId newPage(Page firstpg, int run_size)

  /**
   * Deallocates a single page from disk, freeing it from the pool if needed.
   * 
   * @param pageno identifies the page to remove
   * @throws IllegalArgumentException if the page is pinned
   */
  public void freePage(PageId pageno) {
	  
	  if (!map.containsKey(new Integer(pageno.pid))){// if page not in buffer
		  Minibase.DiskManager.deallocate_page(pageno);
	  }
	  else{
		  int idx = map.get(new Integer(pageno.pid)).intValue();
		  if (frametab[idx].getPinCount()>0){//  exception
			  throw new IllegalArgumentException("page is pinned!");
			  }
		  else{
			  
			  if (frametab[idx].isDirty()&& frametab[idx].getValid()) { flushPage(pageno);}
			  
		      Minibase.DiskManager.deallocate_page(pageno);
		      map.remove(new Integer(pageno.pid));
		      frametab[idx].setPageId(new PageId());//reset frametab status
		     
		      frametab[idx].setRef(false);
		      frametab[idx].setDirty(false);
		      frametab[idx].setValid(false);
		     
		     }
	  }

  } // public void freePage(PageId firstid)

  /**
   * Write all valid and dirty frames to disk.
   * Note flushing involves only writing, not unpinning or freeing
   * or the like.
   * 
   */
  public void flushAllFrames() {

    for (int i =0; i<total_numframes; i++){
    	if(frametab[i].getValid() && frametab[i].isDirty()){
    		flushPage(frametab[i].getPageId());
    	}
    }

  } // public void flushAllFrames()

  /**
   * Write a page in the buffer pool to disk, if dirty.
   * 
   * @throws IllegalArgumentException if the page is not in the buffer pool
   */
  public void flushPage(PageId pageno) {
	  if (!map.containsKey(new Integer(pageno.pid))) throw new IllegalArgumentException("page not in buffer pool!");
	  else {
		
		  int idx = map.get(new Integer(pageno.pid)).intValue();  
	      Minibase.DiskManager.write_page(pageno, BufPool[idx]);
          frametab[idx].setDirty(false);
	  }
  }

   /**
   * Gets the total number of buffer frames.
   */
  public int getNumFrames() {  
    return total_numframes;
  }

  /**
   * Gets the total number of unpinned buffer frames.
   */
  public int getNumUnpinned() {
    
	  int count = 0;
      for(int i = 0; i < total_numframes; i++) {
    	  if(frametab[i].getPinCount() == 0)
    		  count++;
      }
      return count;

  }

  /* Clock algorithm needs to access the frame table */
  public FrameDesc[] frametab(){
	  return frametab;
  }
  
} // public class BufMgr implements GlobalConst
