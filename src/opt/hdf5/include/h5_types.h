// -*- C++ -*-
/**
   
   @file opt/hdf5/include/types.h
   @brief Declaration of hdf5 related library types

   This file contains the declaration of all hdf5 related library types

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __h5_types_h__
#define __h5_types_h__

namespace CNORXZ
{
    namespace hdf5
    {
	// definition -> h5_content_base.h
	class ContentBase;

	// definition -> h5_group.h
	class Group;

	// definition -> h5_file.h
	class File;

	// definition -> h5_table.h
	class Table;

	// definition -> h5_table.h
	template <typename... Ts>
	class STable;

	// definition -> h5_dataset.h
	class Dataset;

	// definition -> h5_dataset.h
	template <typename T>
	class SDataset;

    }
}

#endif
