#include "MemeField.h"

#include <cassert>
#include <random>

void MemeField::Tile::SpawnMeme()
{
	assert( !hasMeme );
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

bool MemeField::Tile::IsRevealed() const
{
	return state == State::REVEALED;
}

bool MemeField::Tile::IsFlagged() const
{
	return state == State::FLAGGED;
}

void MemeField::Tile::Reveal()
{
	assert( state == State::HIDDEN );
	state = State::REVEALED;
}

void MemeField::Tile::Flag()
{
	assert( state == State::HIDDEN );
	state = State::FLAGGED;
}

void MemeField::Tile::Unflag()
{
	assert( state == State::FLAGGED );
	state = State::HIDDEN;
}

MemeField::Tile& MemeField::TileAt( const Vei2& gridPos )
{
	assert(
		gridPos.x >= 0 &&
		gridPos.x < GRID_WIDTH &&
		gridPos.y >= 0 &&
		gridPos.y < GRID_HEIGHT );
	return field[ GRID_WIDTH * gridPos.y + gridPos.x ];
}

const MemeField::Tile& MemeField::TileAt( const Vei2& gridPos ) const
{
	assert(
		gridPos.x >= 0 &&
		gridPos.x < GRID_WIDTH &&
		gridPos.y >= 0 &&
		gridPos.y < GRID_HEIGHT );
	return field[ GRID_WIDTH * gridPos.y + gridPos.x ];
}

MemeField::MemeField( int nMemes )
{
	std::random_device rd;
	std::mt19937 rng( rd() );
	std::uniform_int_distribution<int> xpos( 0,GRID_WIDTH - 1 );
	std::uniform_int_distribution<int> ypos( 0,GRID_HEIGHT - 1 );

	for( ; nMemes > 0; --nMemes )
	{
		Vei2 gridPos;
		do
		{
			gridPos = { xpos( rng ),ypos( rng ) };
		}
		while( TileAt( gridPos ).HasMeme() );
		TileAt( gridPos ).SpawnMeme();
	}
}
