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
// Pid.h: interface for the TPid template class
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PID_H__508AEC09_C6F1_4EC2_A403_19F562532244__INCLUDED_)
#define AFX_PID_H__508AEC09_C6F1_4EC2_A403_19F562532244__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//typedef float PIDTYPE;

template < class T > class TPid
{

public:
	TPid( T dSensitivity = 2 )
	{
		m_tI = 0;
		m_tIt = dSensitivity;

		m_lv = 0;
		m_v = 0;

		m_le = 0;

		m_lUpdates = 0;
	}

	virtual ~TPid()
	{
	}


public:

	operator T() { return m_v; }
	T operator = ( T v ) { return m_lv = m_v = v; }

	T operator += ( T v ) { return Pid( v, 1 ); }
	T Pid( T v, T tSkip = 1 )
	{
		T e = ( v - m_v ) / tSkip;
		
		// Track error
		if ( !m_lUpdates ) m_v = v, m_tI = e;
		else 
		{
			// Track error
			m_le = e - m_tI;
			m_tI = ( ( m_tI * ( m_tIt - 1 ) ) + e ) / m_tIt;

			// Next value
			Next();

		} // end else

		m_lUpdates++;

		return m_v;
	}

	T Next( T i = 1 )
	{	//m_tI += ( m_le * i );
		return m_v = ( m_v + ( m_tI * i ) );
	}

private:

	T			m_tI;
	T			m_tIt;
	
	T			m_le;
	T			m_lv;

	T			m_v;

	long		m_lUpdates;

};


/*

/*  PIDTYPE     m_fKp;
    PIDTYPE     m_fTi;
    PIDTYPE     m_fTs;
    PIDTYPE     m_fIntegral;
    PIDTYPE     m_fHigh;
    PIDTYPE     m_fLow;
    PIDTYPE     m_fTd;
    PIDTYPE     m_fTf;
    PIDTYPE     m_fDifOld;
    PIDTYPE     m_fErrOld;
* /

CPid< class T >::CPid()
{
    m_fKp = 0.f;
    m_fTi = 1.f;
    m_fTs = 1.f;
    m_fIntegral = 0.f;

    m_fHigh = 0.f;
    m_fLow = 0.f;

    m_fTd = 0.f;
    m_fTf = 0.f;

    m_fDifOld = 0.f;
    m_fErrOld = 0.f;

	m_lv = 0;
	m_v = 0;
}

CPid< class T >::~CPid()
{

}

T CPid< class T >::Pid(T v)
{
	float e, d, k;

	m_lv = v;
	e = v - m_v;

	// Sanity checks
	if ( m_fTi == 0 ) m_fTi = 1;

	// Calculate integral
	m_fIntegral = m_fIntegral + ( m_fTs * e ) / m_fTi;
	if ( m_fIntegral > m_fHigh ) m_fIntegral = m_fHigh;
	else if ( m_fIntegral < m_fLow ) m_fIntegral = m_fLow;
	
	if ( m_fTd )
	{
		k = m_fTf + m_fTs;

		if ( k == 0.f ) d = e;
		else d = ( ( e - m_fErrOld ) * m_fTd + m_fTf * m_fDifOld ) / k;

		if ( d > m_fHigh ) d = m_fHigh;
		else if ( d < - m_fHigh ) d = -m_fHigh;

		m_fErrOld = e;
		m_fDifOld = d;

		m_v = m_fIntegral + m_fKp * e + d;

	} // end if

	else m_v = m_fIntegral + m_fKp * e;

	if ( m_v > m_fHigh ) m_v = m_fHigh;
	else if ( m_v < m_fLow ) m_v = m_fLow;


	return m_v;
}
*/

#endif // !defined(AFX_PID_H__508AEC09_C6F1_4EC2_A403_19F562532244__INCLUDED_)
