
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
    segment_end(dest);
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::curve(vector_type control, vector_type dest) -> Loop&
{
    m_points.push_back(control);
    segment_end(dest);
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::curve(vector_type control1, vector_type control2, vector_type dest) -> Loop&
{
    m_points.push_back(control1);
    m_points.push_back(control2);
    segment_end(dest);
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::points() const -> std::vector<vector_type>
{
    return m_points;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::segments() const -> std::vector<index_type>
{
    return m_segments;
}

template <typename Vector_t, typename Index_t>
void Loop<Vector_t, Index_t>::segment_end(vector_type dest)
{
    m_points.push_back(dest);
    m_segments.push_back(m_points.size());
}
