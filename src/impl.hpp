
#pragma mark Loop Impl

template <typename Vector_t, typename Index_t>
Loop<Vector_t, Index_t>::Loop(vector_type begin)
    : m_points(1, begin)
    , m_segments()
{
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::line(vector_type dest) -> Loop&
{
    m_points.push_back(dest);
    m_segments.push_back(2);
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::curve(vector_type control, vector_type dest) -> Loop&
{
    m_points.push_back(control);
    m_points.push_back(dest);
    m_segments.push_back(3);
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::curve(vector_type control1, vector_type control2, vector_type dest) -> Loop&
{
    m_points.push_back(control1);
    m_points.push_back(control2);
    m_points.push_back(dest);
    m_segments.push_back(4);
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::close() -> Loop&
{
    if (points().front() == points().back())
    {
        points().pop_back();
    }
    else {
        m_segments.push_back(2);
    }
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::points() -> std::vector<vector_type> &
{
    return m_points;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::points() const -> std::vector<vector_type>
{
    return m_points;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::segments() -> std::vector<index_type> &
{
    return m_segments;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::segments() const -> std::vector<index_type>
{
    return m_segments;
}



#pragma mark Comp

template <typename Loop_t>
Comp<Loop_t>::Comp(vector_type size)
    : m_size(size)
    , m_loops()
{
}

template <typename Loop_t>
Comp<Loop_t>& Comp<Loop_t>::insert(loop_type l)
{
    m_loops.push_back(l);
    return *this;
}

template <typename Loop_t>
auto Comp<Loop_t>::size() -> vector_type&
{
    return m_size;
}

template <typename Loop_t>
auto Comp<Loop_t>::size() const -> vector_type
{
    return m_size;
}

template <typename Loop_t>
auto Comp<Loop_t>::loops() -> std::vector<Loop_t>&
{
    return m_loops;
}

template <typename Loop_t>
auto Comp<Loop_t>::loops() const -> std::vector<Loop_t>
{
    return m_loops;
}
