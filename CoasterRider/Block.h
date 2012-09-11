#ifndef BLOCK_H
#define BLOCK_H

class Block
	{
	public:	//types
		typedef enum Type {	bt_trim=0,bt_lift,bt_target	};
	public:
		float startpos;			// the progression distance at which the block starts
		float len;					// the progression length of the block from start
		Type type;
		float targetspeed;			/// the target speed of a train
		float&minspeed;			/// the minimum speed of a non-blocked train
		float&maxspeed;			/// the maximum speed of a non-blocked train
		float controlaccel;		/// the amount that the current speed speeds up or
		                        /// ...slows down to get to target speed
	public:
		Block();
		virtual ~Block();
	protected:
	private:
	};

#endif // BLOCK_H
