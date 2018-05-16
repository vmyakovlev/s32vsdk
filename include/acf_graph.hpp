/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

#ifndef ACFGRAPH_HPP
#define ACFGRAPH_HPP

#include <acf_common.h>
#include <../libs/apex/acf_offline/include/acf_node.hpp>
#include <../libs/apex/acf_offline/include/acf_kernel.hpp>
#include <stdint.h>
#include <string>

#define ACF_MAX_NUM_KERNELS_PER_GRAPH 100

//////////////////////////////////////////////////////////////////////////
/// ACF_Graph is a base class designed to encapsulate an ACF graph.  In
/// order to create a graph, a user must derive from this class and
/// implement the pure virtual Create() method.
//////////////////////////////////////////////////////////////////////////
class ACF_Graph : public ACF_Node
{
   friend class ACF_Process_Desc;
   friend class ACF_Process_Desc_APU;

public:

   ACF_Graph();
   ~ACF_Graph();

   //Create() cannot really have a useful return value without having a bunch of error checking code
   //cluttering up the graph description, so no return here. If something goes wrong it
   //will be tracked and indicated by an internal state variable.

   /////////////////////////////////////////////////////////////////////////
   /// This is a pure virtual method that must be implemented by the
   /// derived class.  Use the graph construction methods of ACF_Graph
   /// (e.g. AddKernel, AddInputPort, AddOutputPort, Connect, etc.),
   /// to describe the graph.
   /////////////////////////////////////////////////////////////////////////
   virtual void Create() = 0;

   //NOTE: The following 5 methods are basically wrappers for ACF_Node versions of these
   //methods; this was done to improve graph interface visibility to the user, hide the
   //underlying class hierarchy, and improve error checking and reporting.

   /////////////////////////////////////////////////////////////////////////
   /// Set an identifier to uniquely identify the graph.
   ///
   /// \param[in] lGraphIdentifier
   /// Graph identifier.
   /////////////////////////////////////////////////////////////////////////
   void SetIdentifier(std::string lGraphIdentifier);

   /////////////////////////////////////////////////////////////////////////
   /// Add an input port identified by "lPortIdentifier" to the graph.
   ///
   /// eg.
   /// \code{.cpp}
   /// AddInputPort("GRAPH_INPUT_0");
   /// AddInputPort("GRAPH_INPUT_1");
   /// \endcode
   ///
   /// \param[in] lPortIdentifier
   /// Input port identifier.
   /////////////////////////////////////////////////////////////////////////
   void AddInputPort(std::string lPortIdentifier);  //wrapper for ACF_Node::AddInputPort

   /////////////////////////////////////////////////////////////////////////
   /// Add an output port identified by "lPortIdentifier" to the graph.
   ///
   /// eg.
   /// \code{.cpp}
   /// AddOutputPort("GRAPH_OUTPUT_0");
   /// AddOutputPort("GRAPH_OUTPUT_1");
   /// \endcode
   ///
   /// \param[in] lPortIdentifier
   /// Output port identifier.
   /////////////////////////////////////////////////////////////////////////
   void AddOutputPort(std::string lPortIdentifier); //wrapper for ACF_Node::AddOutputPort

   /////////////////////////////////////////////////////////////////////////
   /// Return a pointer to the graph port identified by "lPortIdentifier".
   ///
   /// \param[in] lPortIdentifier
   /// Graph port identifier.
   ///
   /// \return
   /// Pointer to the graph port identified by "lPortIdentifier".
   /////////////////////////////////////////////////////////////////////////
   ACF_Port* GraphPort(std::string lPortIdentifier);

   /////////////////////////////////////////////////////////////////////////
   /// Returns a pointer to the port "lPortIdentifier" belonging to the
   /// kernel "lKernelIdentifier".
   ///
   /// \param[in] lKernelIdentifier
   /// Kernel instance identifier.
   /// \param[in] lPortIdentifier
   /// Kernel port identifier.
   ///
   /// \return
   /// Pointer to the port "lPortIdentifier" belonging to the kernel
   /// "lKernelIdentifier".
   /////////////////////////////////////////////////////////////////////////
   ACF_Port* KernelPort(std::string lKernelIdentifier, std::string lPortIdentifier);

   /////////////////////////////////////////////////////////////////////////
   /// Creates an instance of the kernel "lKernelDatabaseIdentifier" in the graph
   /// (this is the unique kernel identifier specified in the kernel metadata).
   /// and assigns the kernel instance the unique handle specified by
   /// "lKernelIdentifier".  Note that if there are N instances of a kernel in a graph,
   /// that kernel must be 'instantiated' N times, each time with a unique "lKernelIdentifier".
   ///
   /// eg.
   /// \code{.cpp}
   /// AddKernel("myAddKernel1",   "ADD"); //first instance of the 'ADD' kernel
   /// AddKernel("myAddKernel2",   "ADD"); //second instance of the 'ADD' kernel
   /// AddKernel("myFilterKernel", "FILTER");
   /// \endcode
   ///
   /// \param[in] lKernelIdentifier
   /// Identifier that acts as a local kernel handle (i.e. the identifier
   /// by which a kernel instance is referred to during graph construction)
   /// \param[in] lKernelDatabaseIdentifier
   /// Identifier used to select kernel from the database.  This is the identifier
   /// specified in the kernel metadata.
   /////////////////////////////////////////////////////////////////////////
   void AddKernel(std::string lKernelIdentifier, std::string lKernelDatabaseIdentifier);

   /////////////////////////////////////////////////////////////////////////
   /// Connect the source port "lpSrcPort" to the destination port "lpDstPort".
   /// This is a forward, directed connection from source to destination.
   ///
   /// \param[in] lpSrcPort
   /// Pointer to source ACF_Port.
   /// \param[in] lpDstPort
   /// Pointer to destination ACF_Port.
   /////////////////////////////////////////////////////////////////////////
   void Connect(ACF_Port* lpSrcPort, ACF_Port* lpDstPort);


   void AddGraph(ACF_Graph& lGraph);
   int32_t RetErrorState();

protected:

   ACF_Kernel* Kernel(std::string lKernelIdentifier); //return pointer to kernel specified by 'lKernelIdentifier'
   ACF_NODE_TYPE RetNodeType();
   ACF_Kernel* TraceBackwardsFromPortToKernel(ACF_Port* lpPort);
   int32_t RetOuputDelay(ACF_Port* lpPort);
   int32_t VerifyPortConnections();

   ACF_Port* SetKernelPortOutputDelay(std::string lKernelIdentifier, std::string lPortIdentifier, int32_t lOutputDelay); //THIS IS TEMPORARY

private:

   int32_t mNumKernels;
   ACF_Kernel* mKernelArray; //allocated on heap w/ new
   int32_t mErrorState;
};

#endif /* ACFGRAPH_HPP */
