/*******************************************************************
// Copyright (c) 2002, Robert Umbehant
// mailto:rumbehant@wheresjames.com
//
// This library is free software; you can redistribute it and/or 
// modify it under the terms of the GNU Lesser General Public 
// License as published by the Free Software Foundation; either 
// version 2.1 of the License, or (at your option) any later 
// version.
//
// This library is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public 
// License along with this library; if not, write to the Free 
// Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
// MA 02111-1307 USA 
//
*******************************************************************/
// WinRect.h: interface for the CWinRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINRECT_H__B6EAF711_4B89_45E7_9664_FAB484954D0B__INCLUDED_)
#define AFX_WINRECT_H__B6EAF711_4B89_45E7_9664_FAB484954D0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//==================================================================
// CWinRect
//
/// Encapsulates a rectangle object
/**
	Simple wrapper.  I should really add a lot more to this class.	
*/
//==================================================================
class CWinRect
{
public:

	/// Constructor
	CWinRect();
	
	/// Destructor
	virtual ~CWinRect();

public:

	//==============================================================
	// CombineOverlapping()
	//==============================================================
	/// Combines an overlapping rectangle list
	/**
		\param [in] pRectList		-	Array of RECT structures
		\param [in] dwRectListSize	-	Rectangles in pRectList
		\param [in] xThreshold		-	Horizontal overlap threshold
		\param [in] yThreshold		-	Vertical overlap threshold
		\param [in] max				-	Maximum number to combine
		\param [in] pdwFree			-	Next free rectangle

		This function does not defrag the rectangle list.
		Free rectangles are marked by setting all their values to zero.
	  
		\return Number of rectangles consolidated
	
		\see 
	*/
	static DWORD CombineOverlapping( LPRECT pRectList, DWORD dwRectListSize, LONG xThreshold, LONG yThreshold, DWORD max = MAXDWORD, LPDWORD pdwFree = NULL );

	//==============================================================
	// IsAdjacentRect()
	//==============================================================
	/// Returns non-zero if the rectangles are 'close' as defined by the threshold
	/**
		\param [in] pRect1		-	First rectangle
		\param [in] pRect2		-	Second rectangle
		\param [in] xThreshold	-	Horizontal threshold
		\param [in] yThreshold	-	Vertical threshold
		
		\return Non-zero if rectangles are 'close' as defined by threshold
	
		\see 
	*/
	static BOOL IsAdjacentRect( LPRECT pRect1, LPRECT pRect2, LONG xThreshold = 0, LONG yThreshold = 0 );

	//==============================================================
	// AddRectToList()
	//==============================================================
	/// Adds a rectangle to a list
	/**
		\param [in] pRect			-	Rectangle to add
		\param [in] pRectList		-	List of rectangles
		\param [in] dwRectListSize	-	Number of rectangles in pRectList
		\param [in] xThreshold		-	Horizontal threshold
		\param [in] yThreshold		-	Vertical threshold

		This function adds a rectangle to the list by scanning for a free
		slot marked by all the rectangle values being set to zero.  It
		also checks each rectangle in the list for one that is 'close'
		as defined by the threshold values.  If a 'close' rectangle is
		found, then this rectangle is added to the 'close' rectangle by
		creating a unioned rectangle.  That is, the smallest rectangle
		that completely encompases both the new rectangle and the 'close'
		rectangle.
		
		\return Non-zero if rectangle was added
	
		\see 
	*/
	static BOOL AddRectToList( LPRECT pRect, LPRECT pRectList, DWORD dwRectListSize, LONG xThreshold = 0, LONG yThreshold = 0 );

	//==============================================================
	// LPRECT()
	//==============================================================
	/// Returns a pointer to the encapsulated rectangle
	operator LPRECT() { return &m_rect; }

	//==============================================================
	// RECT()
	//==============================================================
	/// Returns a reference to the encapsulated rectangle
	operator RECT&() { return m_rect; }

	//==============================================================
	// Width()
	//==============================================================
	/// Returns the width of the rectangle
	LONG Width() { return Width( &m_rect ); }

	//==============================================================
	// Width()
	//==============================================================
	/// Returns the width of the rectangle
	static LONG Width( LPRECT pRect )
	{	return ( pRect->right - pRect->left ); }

	//==============================================================
	// Height()
	//==============================================================
	/// Returns the height of the rectangle
	LONG Height() { return Height( &m_rect ); }

	//==============================================================
	// Height()
	//==============================================================
	/// Returns the height of the rectangle
	static LONG Height( LPRECT pRect )
	{	return ( pRect->bottom - pRect->top ); }

private:

	/// Encapsulated rectangle object
	RECT			m_rect;

};

#endif // !defined(AFX_WINRECT_H__B6EAF711_4B89_45E7_9664_FAB484954D0B__INCLUDED_)
