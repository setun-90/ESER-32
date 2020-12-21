#ifndef   _TRACE_H
#define   _TRACE_H



/* Überschrift hier */

#define TRACE(X) std::cerr <<  __FILE__ << ':' << std::dec << __LINE__ << ": " << (X) << '\n'



#endif /* _TRACE_H */
