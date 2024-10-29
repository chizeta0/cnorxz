// -*- C++ -*-
/**

   @file opt/hdf5-mpi/include/h5_rdataset.h
   @brief RDataset declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_rdataset_h__
#define __cxz_h5_rdataset_h__

namespace CNORXZ
{
    namespace hdf5
    {
	/** ****
	    Class to handle hdf5 datasets on multiple ranks.
	 */
	template <typename T>
	class RDataset : public SDataset<T>
	{
	public:
	    DEFAULT_MEMBERS(RDataset); /**< Default constructors and assignments. */

	    /** Construct the class.
		@param name Dataset name.
		@param _parent Parent content object.
	     */
	    RDataset(const String& name, const ContentBase* _parent);
	    
	    /** Initalize the dataset.
		@param data Array containing the dataset.
	    */
	    template <typename T>
	    RDataset& init(const mpi::RArray<T>& data);

	    /** Read the dataset.
		@return Array containing the dataset values.
	     */
	    mpi::RArray<T> read() const;

	    /** Read a given subset of the dataset.
		The subset needs to be hypercubic.
		@param beg Index indicating the global begin edge of the hypercube.
		@param end Index indicating the global end edge of the hypercube (inclusive).
		@return Array containing the dataset values.
	     */
	    template <class I, typename M>
	    mpi::RArray<T> read(const IndexInterface<I,M>& beg, const IndexInterface<I,M>& end) const;
	    
	};

#endif
