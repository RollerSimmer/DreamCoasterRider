#ifndef BLOCK_H
#define BLOCK_H


class Block
	{
	public:
		float startpos;			// the progression distance at which the block starts
		float len;					// the progression length of the block from start
		bool doliftflag;
	public:
		Block();
		virtual ~Block();
	protected:
	private:
	};

#endif // BLOCK_H
