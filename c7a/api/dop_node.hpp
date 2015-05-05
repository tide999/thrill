/*******************************************************************************
 * c7a/api/dop_node.hpp
 *
 * Model real-time or backtesting Portfolio with Positions, TradeLog and more.
 ******************************************************************************/

#ifndef C7A_API_DOP_NODE_HEADER
#define C7A_API_DOP_NODE_HEADER

#include "dia_node.hpp"
#include "context.hpp"

namespace c7a {
template <typename T>
class DOpNode : public DIANode<T>
{
public:
    DOpNode(Context& ctx,
            const std::vector<DIABase*>& parents)
        : DIANode<T>(ctx, parents) { }
    virtual ~DOpNode() { }
};
} // namespace c7a

#endif // !C7A_API_DOP_NODE_HEADER

/******************************************************************************/
