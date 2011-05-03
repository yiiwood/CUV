#ifndef __ACCCESSORS_HPP__
#define __ACCCESSORS_HPP__
#include <cuv/tools/cuv_general.hpp>

namespace cuv
{
	/// Allocator allows allocation, deallocation and copying depending on memory_space_type
	template <class value_type, class index_type, class memory_space_type>
		struct allocator{
			void alloc( value_type** ptr, index_type memsize)const; /// allocate memory 
			void alloc2d( value_type** ptr, index_type& pitch, index_type height, index_type width)const; /// allocate memory 
			void dealloc( value_type** ptr)const;                   /// free memory
			void copy(value_type* dst, const value_type*src,index_type size, host_memory_space); /// copy from host_memory_space
			void copy(value_type* dst, const value_type*src,index_type size, dev_memory_space);  /// copy from dev_memory_space

			void copy2d(value_type* dst, const value_type*src,index_type dpitch, index_type spitch, index_type h, index_type w, host_memory_space); /// copy from host_memory_space
			void copy2d(value_type* dst, const value_type*src,index_type dpitch, index_type spitch, index_type h, index_type w, dev_memory_space);  /// copy from dev_memory_space

			void alloc( const value_type** ptr, index_type memsize)const;   /// const variant of the above
			void dealloc( const value_type** ptr)const;                     /// const variant of the above
			void copy(const value_type* dst, const value_type*src,index_type size, host_memory_space)const{cuvAssert(false);}   /// this throws an assertion, it should never be called
			void copy(const value_type* dst, const value_type*src,index_type size, dev_memory_space)const{cuvAssert(false);};   /// this throws an assertion, it should never be called
		};

	/**
	 * Set the value at *(ptr+idx) to val, when ptr is in host_memory_space.
	 */
	template <class value_type, class index_type>
		void entry_set(value_type* ptr, index_type idx, value_type val, host_memory_space);
	/**
	 * Get the value at *(ptr+idx), when ptr is in host_memory_space.
	 */
	template <class value_type, class index_type>
		value_type entry_get(const value_type* ptr, index_type idx, host_memory_space);

	/**
	 * Set the value at *(ptr+idx) to val, when ptr is in dev_memory_space.
	 */
	template <class value_type, class index_type>
		void entry_set(value_type* ptr, index_type idx, value_type val, dev_memory_space);
	/**
	 * Get the value at *(ptr+idx), when ptr is in dev_memory_space.
	 */
	template <class value_type, class index_type>
		value_type entry_get(const value_type* ptr, index_type idx, dev_memory_space);


	/** 
	 * @brief Setting entry of host linear_memory at ptr at index idx to value val
	 * 
	 * @param ptr Address of array in memory
	 * @param idx Index of value to set
	 * @param val Value to set linear_memory entry to
	 * 
	 */
	template <class value_type, class index_type>
		void entry_set(value_type* ptr, index_type idx, value_type val, host_memory_space) {
			ptr[idx]=val;
		}


	/** 
	 * @brief Getting entry of host linear_memory at ptr at index idx
	 * 
	 * @param ptr Address of array in memory
	 * @param idx Index of value to get
	 * 
	 * @return 
	 */
	template <class value_type, class index_type>
		value_type entry_get(const value_type* ptr, index_type idx, host_memory_space) {
			return ptr[idx];
		}

	/**
	 * @brief specialization of template allocator for host_memory_space
	 *
	 * The gcc compiler can deal directly with this, so we do not need to hide the implementation in a
	 * .cu file.
	 */
	template<class value_type, class index_type>
		struct allocator<value_type,index_type,host_memory_space>{
			/** 
			 * @brief Allocate memory for host matrices 
			 * 
			 * @param ptr Address of pointer which will be set to allocated memory
			 * @param size Size of array which should be allocated
			 * 
			 * This is the instance of the alloc function that is called by host vectors.
			 */
			void alloc( value_type** ptr, index_type size) const{
				*ptr = new value_type[size];
			}
			/**
			 * @brief Allocate 2D memory
			 *
			 * @param ptr     Address of pointer which will be set to allocated memory
			 * @param pitch  OUT size of one "line" in memory in bytes
			 * @param height  number of lines in memory
			 * @param width   number elements in one line
			 */
			void alloc2d( value_type** ptr, index_type& pitch, index_type height, index_type width)const; /// allocate memory 
			/** 
			 * @brief Deallocate memory for host matrices
			 * 
			 * @param ptr Address of pointer that will be freed
			 * 
			 * This is the instance of the dealloc function that is called by host vectors.
			 */
			void dealloc( value_type** ptr)const {
				delete[] *ptr;
				*ptr = 0;
			}
			/** 
			 * @brief Allocate memory for host matrices - const allocator should never be called!
			 * 
			 * @param ptr Address of pointer which will be set to allocated memory
			 * @param size Size of array which should be allocated
			 * 
			 * This is the instance of the alloc function that is called by const host vectors.
			 */
			void alloc(const value_type** ptr, index_type size) const{
				cuvAssert(false);
			}
			/** 
			 * @brief Deallocate memory for host matrices- const allocator should never be called!
			 * 
			 * @param ptr Address of pointer that will be freed
			 * 
			 * This is the instance of the dealloc function that is called by host vectors.
			 */
			void dealloc(const value_type** ptr)const {
				cuvAssert(false);
			}
			/// copy from host to host
			void copy(value_type* dst, const value_type*src,index_type size, host_memory_space){
				memcpy(dst,src,size*sizeof(value_type));
			}
			/// copy from device to host
			void copy(value_type* dst, const value_type*src,index_type size, dev_memory_space);

			/// throw assertion (should never be called)
			void copy(const value_type* dst, const value_type*src,index_type size, host_memory_space)const{
				cuvAssert(false);
			}
			/// throw assertion (should never be called)
			void copy(const value_type* dst, const value_type*src,index_type size, dev_memory_space)const{
				cuvAssert(false);
			}

			void copy2d(value_type* dst, const value_type*src,index_type dpitch, index_type spitch, index_type h, index_type w, host_memory_space); /// copy from host_memory_space
			void copy2d(value_type* dst, const value_type*src,index_type dpitch, index_type spitch, index_type h, index_type w, dev_memory_space);  /// copy from dev_memory_space
		};

}


#endif /* __ACCCESSORS_HPP__ */
