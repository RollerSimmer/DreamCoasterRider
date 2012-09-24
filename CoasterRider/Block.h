#ifndef BLOCK_H
#define BLOCK_H

class Block
	{
	public:	//types
		typedef enum Type  ///block types for different modes of operation								   
			{	 
 			 bt_normal=0	///normal (gravity) block
         ,bt_trim			///trim brakes (or lift to hit target speed
         ,bt_lift			///lift block
		   ,bt_boost		///boost block
		   ,bt_station		///station block
		   // Note: some may block types behave similarly, but need to have a different type enum
		   //         for graphics and sound
			}; 
	public:
		float startpos;		/// the progression distance at which the block starts
		float len;				/// the progression length of the block from start
		Type type;				/// the type of block;
		float forespeed;		/// the target speed for forward stages
		float backspeed;    	/// the target speed for backward stages
		float foreaccel;		/// the acceleration to get to target for forward stages
		float backaccel;		/// the acceleration to get to target for backward stages
	public:
		Block();
		virtual ~Block();
	protected:
	private:
	};

#endif // BLOCK_H
