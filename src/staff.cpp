/////////////////////////////////////////////////////////////////////////////
// Name:        staff.cpp
// Author:      Laurent Pugin
// Created:     2005
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////


#include "staff.h"

//----------------------------------------------------------------------------

#include <assert.h>

//----------------------------------------------------------------------------

#include "vrv.h"
#include "aligner.h"
#include "vrvdef.h"
#include "io.h"
#include "layer.h"
#include "system.h"

namespace vrv {

//----------------------------------------------------------------------------
// Staff
//----------------------------------------------------------------------------

Staff::Staff( int n ):
	DocObject("staff-")
{
	Clear( );
    //wxASSERT ( n > 0 );
    m_n = n;
}

Staff::Staff( const Staff& staff )
{
    m_parent = NULL;
	totGrp = staff.totGrp;
	//noLigne = staff.noLigne;
	armTyp = staff.armTyp;
	armNbr = staff.armNbr;
	notAnc = staff.notAnc;
	grise = staff.grise;
	invisible = staff.invisible;
	vertBarre = staff.vertBarre;
	brace = staff.brace;
	staffSize = staff.staffSize;
	portNbLine = staff.portNbLine;
	accol = staff.accol;
	accessoire = staff.accessoire;
	m_yAbs = staff.m_yAbs;
	m_drawingY = staff.m_drawingY;
    m_staffAlignment = NULL;

    int i;
	for (i = 0; i < staff.GetLayerCount(); i++)
	{
        Layer *nlayer = new Layer( *dynamic_cast<Layer*>( staff.m_children[i] ) );
        this->AddLayer( nlayer );
	}
    
    this->ResetUuid();
}

Staff::~Staff()
{
    
}

void Staff::Clear()
{
	ClearChildren();
    m_parent = NULL;
	noGrp = 0;
	totGrp = 0;
	//noLigne = 0; // ax2
	armTyp = 0;
	armNbr = 0;
	notAnc = false; // LP we want modern notation :))
	grise = false;
	invisible = false;
	vertBarre = 0;
	brace = 0;
	staffSize = 0; 
	portNbLine = 5;
	accol = 0;
	accessoire = 0;
	m_yAbs = VRV_UNSET;
	m_drawingY = 0;
    m_staffAlignment = NULL;
}

int Staff::Save( ArrayPtrVoid params )
{
    // param 0: output stream
    FileOutputStream *output = static_cast<FileOutputStream*>(params[0]);           
    if (!output->WriteStaff( this )) {
        return FUNCTOR_STOP;
    }
    return FUNCTOR_CONTINUE;

}

void Staff::AddLayer( Layer *layer )
{
	layer->SetParent( this );
	m_children.push_back( layer );
    
    if ( layer->GetLayerNo() == -1 ) {
        layer->SetLayerNo( this->GetLayerCount() );
    }
}

void Staff::CopyAttributes( Staff *nstaff )
{
	if ( !nstaff )
		return;

	nstaff->Clear();
	nstaff->noGrp = noGrp;
	nstaff->totGrp = totGrp;
	//nstaff->noLigne = noLigne;
	nstaff->armTyp = armTyp;
	nstaff->armNbr = armNbr;
	nstaff->notAnc = notAnc;
	nstaff->grise = grise;
	nstaff->invisible = invisible;
	nstaff->vertBarre = vertBarre;
	nstaff->brace = brace;
	nstaff->staffSize = staffSize;
	nstaff->portNbLine = portNbLine;
	nstaff->accol = accol;
	nstaff->accessoire = accessoire;
	nstaff->m_yAbs = m_yAbs;
	nstaff->m_drawingY = m_drawingY;
}

Layer *Staff::GetFirst( )
{
	if ( m_children.empty() )
		return NULL;
	return dynamic_cast<Layer*>(m_children[0]);
}

Layer *Staff::GetLast( )
{
	if ( m_children.empty() )
		return NULL;
	int i = GetLayerCount() - 1;
	return dynamic_cast<Layer*>(m_children[i]);
}

Layer *Staff::GetNext( Layer *layer )
{	
    if ( !layer || m_children.empty())
        return NULL;
        
	int i = GetChildIndex( layer );

	if ((i == -1 ) || ( i >= GetLayerCount() - 1 ))
		return NULL;

	return dynamic_cast<Layer*>(m_children[i + 1]);
}

Layer *Staff::GetPrevious( Layer *layer )
{
    if ( !layer || m_children.empty())
        return NULL;
        
	int i = GetChildIndex( layer );

	if ((i == -1 ) || ( i <= 0 ))
        return NULL;
	
    return dynamic_cast<Layer*>(m_children[i - 1]);
}


Layer *Staff::GetLayerWithIdx( int LayerIdx )
{
    if ( LayerIdx > (int)m_children.size() - 1 )
        return NULL;
	
	return dynamic_cast<Layer*>(m_children[LayerIdx]);
}

int Staff::GetVerticalSpacing()
{
    return 160; // arbitrary generic value
}
    
void Staff::ResetDrawingValues()
{
    m_drawingY = 0;
}

bool Staff::GetPosOnPage( ArrayPtrVoid params )
{
    // param 0: the Staff we are looking for
    // param 1: the position on the page (int)
    // param 2; the success flag (bool)
    Staff *staff = static_cast<Staff*>(params[0]);
	int *position = static_cast<int*>(params[1]);
    bool *success = static_cast<bool*>(params[2]);
    
    if ( (*success) ) {
        return true;
    } 
    (*position)++;
    if ( this == staff ) {
        (*success) = true;
        return true;
    }
    // to be verified
    return false;
}


int Staff::GetYRel()
{
    if (m_staffAlignment) {
        return m_staffAlignment->GetYRel();
    }
    return 0;
}

//----------------------------------------------------------------------------
// Staff functor methods
//----------------------------------------------------------------------------


int Staff::Align( ArrayPtrVoid params )
{
    // param 0: the measureAligner (unused)
    // param 1: the time (unused)
    // param 2: the systemAligner
    // param 3: the staffNb
    SystemAligner **systemAligner = static_cast<SystemAligner**>(params[2]);
	int *staffNb = static_cast<int*>(params[3]);
    
    // this gets (or creates) the measureAligner for the measure
    StaffAlignment *alignment = (*systemAligner)->GetStaffAlignment( *staffNb );
    
    assert( alignment );
    
    // Set the pointer of the m_alignment
    m_staffAlignment = alignment;
    
    // for next staff
    (*staffNb)++;
    
    return FUNCTOR_CONTINUE;
}

} // namespace vrv
