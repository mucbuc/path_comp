
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
    m_segments.push_back(1);
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::curve(vector_type control, vector_type dest) -> Loop&
{
    m_points.push_back(control);
    m_points.push_back(dest);
    m_segments.push_back(2);
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::curve(vector_type control1, vector_type control2, vector_type dest) -> Loop&
{
    m_points.push_back(control1);
    m_points.push_back(control2);
    m_points.push_back(dest);
    m_segments.push_back(3);
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::close() -> Loop&
{
    if (points().front() == points().back()) {
        points().pop_back();
    } else {
        m_segments.push_back(1);
    }
    return *this;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::points() -> std::vector<vector_type>&
{
    return m_points;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::points() const -> std::vector<vector_type>
{
    return m_points;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::segments() -> std::vector<index_type>&
{
    return m_segments;
}

template <typename Vector_t, typename Index_t>
auto Loop<Vector_t, Index_t>::segments() const -> std::vector<index_type>
{
    return m_segments;
}

template <typename Vector_t, typename Index_t>
template <typename C>
Loop<C, Index_t> Loop<Vector_t, Index_t>::convert_to() const
{
    Loop<C, Index_t> result;

    result.m_points.reserve(m_points.size());
    for (auto point : m_points) {
        result.m_points.push_back(C { { point[0], point[1] } });
    }

    result.m_segments.reserve(m_segments.size());
    for (auto segments : m_segments) {
        result.m_segments.push_back(segments);
    }

    return result;
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

template <typename Loop_t>
template <typename C>
Comp<Loop<C>> Comp<Loop_t>::convert_to() const
{
    Comp<Loop<C>> result(C { { m_size[0], m_size[1] } });
    result.m_loops.reserve(m_loops.size());
    for (auto loop : m_loops) {
        result.m_loops.push_back(loop.template convert_to<C>());
    }
    return result;
}

#pragma mark Bounds

template <typename Vector_t>
bool Bounds<Vector_t>::operator>(vector_type p) const
{
    const bool accept_x = m_min[0] < p[0] || m_max[0] > p[0];
    const bool accept_y = m_min[1] < p[1] || m_max[1] > p[1];
    return accept_x && accept_y;
}

template <typename Vector_t>
bool Bounds<Vector_t>::operator>=(vector_type p) const
{
    const bool accept_x = m_min[0] <= p[0] || m_max[0] >= p[0];
    const bool accept_y = m_min[1] <= p[1] || m_max[1] >= p[1];
    return accept_x && accept_y;
}

template <typename Vector_t>
bool Bounds<Vector_t>::operator<(vector_type p) const
{
    const bool accept_x = m_min[0] > p[0] || m_max[0] < p[0];
    const bool accept_y = m_min[1] > p[1] || m_max[1] < p[1];
    return accept_x || accept_y;
}

template <typename Vector_t>
bool Bounds<Vector_t>::operator<=(vector_type p) const
{
    const bool accept_x = m_min[0] >= p[0] || m_max[0] <= p[0];
    const bool accept_y = m_min[1] >= p[1] || m_max[1] <= p[1];
    return accept_x || accept_y;
}

#pragma mark non member functions

template <typename Vector_t, typename Index_t>
void reverse(Loop<Vector_t, Index_t>& loop)
{
    std::reverse(loop.points().begin(), loop.points().end());
}

template <typename Vector_t, typename Index_t>
path_comp::Loop<Vector_t, Index_t> make_frame(Bounds<Vector_t> bounds, typename Vector_t::value_type offset)
{
    using loop_type = path_comp::Loop<Vector_t, Index_t>;

    const auto left = bounds.m_min[0] + offset;
    const auto right = bounds.m_max[0] - offset;
    const auto top = bounds.m_min[1] + offset;
    const auto bottom = bounds.m_max[1] - offset;

    const auto result = loop_type { { left, top } }
                            .line({ left, bottom })
                            .line({ right, bottom })
                            .line({ right, top })
                            .close();
    return result;
}

template <class T>
static std::ostream& write_vector(T p, std::ostream& o)
{
    o << p[0] << " " << p[1];
    return o;
}

template <typename T, typename U>
static void start_loop(const T& builder, U id, std::ostream& result)
{
    result << "<path id=\"" << id << "\" d=\"M";
    write_vector(builder.points()[0], result);
}

static void end_loop(std::ostream& result)
{
    result << "Z\"";
    result << "/>\n";
}

template <typename T, typename U>
static void end_segment(const T& points, U begin, U size, std::ostream& result)
{
    switch (size) {
    case 1:
        result << " L";
        break;
    case 2:
        result << " Q";
        break;
    case 3:
        result << " C";
        break;
    default:
        ASSERT(false)(begin)(size);
    }

    while (size) {
        result << " ";
        write_vector(points[begin % points.size()], result);
        --size;
        ++begin;
    }
}

template <typename T>
std::string make_html_svg(const Comp<T> & comp, std::string name)
{
    std::stringstream result;

    for (const auto & loop : comp.loops())
    {
        start_loop(loop, name, result);
        typename T::index_type segment_begin = 1;
        for (auto segment_size : loop.segments())
        {
            end_segment(loop.points(), segment_begin, segment_size, result);
            segment_begin += segment_size;
        }

        end_loop(result);
    }
    return result.str();
}
