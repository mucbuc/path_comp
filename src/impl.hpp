#pragma mark Vector_Builder

template <int>
struct Vector_Builder;

template <>
struct Vector_Builder<2> {
    template <class T>
    static std::array<T, 2> make_vector(T* p)
    {
        return { p[0], p[1] };
    }

    template <class T>
    static std::ostream& write_vector(T p, std::ostream& o)
    {
        o << p[0] << " " << p[1];
        return o;
    }
};

template <>
struct Vector_Builder<3> {
    template <class T>
    static std::array<T, 3> make_vector(T* p)
    {
        return { p[0], p[1], p[2] };
    }

    template <class T>
    static std::ostream& write_vector(T p, std::ostream& o)
    {
        o << p[0] << " " << p[1] << " " << p[2];
        return o;
    }
};

#pragma mark Builder::Loop Impl

template <typename scalar_type, int rank, typename index_type>
Builder<scalar_type, rank, index_type>::Loop::Loop(scalar_type* begin)
{
    m_points.push_back(Vector_Builder<rank>::make_vector(begin));
}

template <typename scalar_type, int rank, typename index_type>
auto Builder<scalar_type, rank, index_type>::Loop::line(scalar_type* dest) -> Loop&
{
    segment_end(Vector_Builder<rank>::make_vector(dest));
    return *this;
}

template <typename scalar_type, int rank, typename index_type>
auto Builder<scalar_type, rank, index_type>::Loop::curve(scalar_type* control, scalar_type* dest) -> Loop&
{
    m_points.push_back(Vector_Builder<rank>::make_vector(control));
    segment_end(Vector_Builder<rank>::make_vector(dest));
    return *this;
}

template <typename scalar_type, int rank, typename index_type>
auto Builder<scalar_type, rank, index_type>::Loop::curve(scalar_type* control1, scalar_type* control2, scalar_type* dest) -> Loop&
{
    m_points.push_back(Vector_Builder<rank>::make_vector(control1));
    m_points.push_back(Vector_Builder<rank>::make_vector(control2));
    segment_end(Vector_Builder<rank>::make_vector(dest));
    return *this;
}

template <typename scalar_type, int rank, typename index_type>
Span<scalar_type> Builder<scalar_type, rank, index_type>::Loop::points() const
{
    return { m_points.data()->data(), m_points.size() * rank };
}

template <typename scalar_type, int rank, typename index_type>
Span<index_type> Builder<scalar_type, rank, index_type>::Loop::segments() const
{
    return { m_segments.data(), m_segments.size() };
}

template <typename scalar_type, int rank, typename index_type>
void Builder<scalar_type, rank, index_type>::Loop::segment_end(vector_type dest)
{
    m_points.push_back(dest);
    m_segments.push_back(m_points.size());
}

#pragma mark Builder Impl

template <typename scalar_type, int rank, typename index_type>
void Builder<scalar_type, rank, index_type>::append(const Loop& l, index_type index)
{
    const auto points_base { m_points.size() };
    m_points.insert(m_points.end(), l.m_points.cbegin(), l.m_points.cend());
    m_indecies.push_back(index);
    auto new_entry { m_segments.insert(m_segments.end(), l.m_segments.cbegin(), l.m_segments.cend()) };

    for (; new_entry != m_segments.end(); ++new_entry) {
        *new_entry += points_base;
    }
    m_loops.push_back(m_segments.size());
}

template <typename scalar_type, int rank, typename index_type>
void Builder<scalar_type, rank, index_type>::append(const Loop& l)
{
    append(l, index_type(m_loops.size()));
}

template <typename scalar_type, int rank, typename index_type>
Span<scalar_type> Builder<scalar_type, rank, index_type>::points() const
{
    return { m_points.data()->data(), m_points.size() * rank };
}

template <typename scalar_type, int rank, typename index_type>
Span<index_type> Builder<scalar_type, rank, index_type>::segments() const
{
    return { m_segments.data(), m_segments.size() };
}

template <typename scalar_type, int rank, typename index_type>
Span<index_type> Builder<scalar_type, rank, index_type>::loops() const
{
    return { m_loops.data(), m_loops.size() };
}

template <typename scalar_type, int rank, typename index_type>
auto Builder<scalar_type, rank, index_type>::indecies() const -> Span<index_type>
{
    return { m_indecies.data(), m_indecies.size() };
}

template <typename scalar_type, int rank, typename index_type>
template <typename T, typename U, typename V>
void Builder<scalar_type, rank, index_type>::traverse(T on_loop_begin, U on_segment_end, V on_loop_end) const
{
    ASSERT(m_indecies.size() == m_loops.size());

    for (
        index_type loop_index = 0, segment_index = 0, point_index = 0;
        loop_index < m_loops.size();
        ++loop_index) {

        on_loop_begin(point_index, m_indecies[loop_index]);

        const index_type point_begin { point_index };
        ++point_index;
        for (; segment_index != m_loops[loop_index]; ++segment_index) {

            const auto segment_end = m_segments[segment_index];
            on_segment_end(point_index, segment_end);
            point_index = segment_end;
        }

        on_loop_end(point_begin);
    }
}

template <typename scalar_type, int rank, typename index_type>
std::ostream& Builder<scalar_type, rank, index_type>::write_point_at(size_t index, std::ostream& out) const
{
    return Vector_Builder<rank>::write_vector(m_points[index].data(), out);
}
