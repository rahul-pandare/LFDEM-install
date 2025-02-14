import numpy as np
import dict_utils as du


def periodize(x, box_lim, lees_edwards_strain, gradient_direction=2):
    """ 
        Periodize x according to Lees-Edwards in a box with limits box_lim.

        x is an array with shape (..., d), box_lim is an array
        with shape (d, 2) containing the min and max box coordinates
        in each direction, and lees_edwards_strain is a vector of size d.

        Note that the Lees-Edwards strain must be 0 along the
        gradient direction.
    """

    if lees_edwards_strain[gradient_direction] != 0:
        raise RuntimeError("lees_edwards_strain[gradient_direction] != 0")

    g = gradient_direction

    box_lim = np.asarray(box_lim)
    box_min = np.array(box_lim[:, 0])
    box_max = np.array(box_lim[:, 1])
    box_size = box_max - box_min

    lees_edwards_disp = np.array(lees_edwards_strain) * box_size[g]

    # start by the gradient direction, easier
    gcrossing_shift = np.array(lees_edwards_disp)
    gcrossing_shift[g] = box_size[g]

    crossing = x[..., g] > box_max[g]
    while crossing.any():
        x[crossing] -= gcrossing_shift
        crossing = x[..., g] > box_max[g]

    crossing = x[..., g] < box_min[g]
    while crossing.any():
        x[crossing] += gcrossing_shift
        crossing = x[..., g] < box_min[g]

    for d in range(x.shape[-1]):
        if d != g:
            crossing_shift = np.zeros(x.shape[-1], dtype=np.float)
            crossing_shift[d] = box_size[d]

            crossing = x[..., d] > box_max[d]
            while crossing.any():
                x[crossing] -= crossing_shift
                crossing = x[..., d] > box_max[d]

            crossing = x[..., d] < box_min[d]
            while crossing.any():
                x[crossing] += crossing_shift
                crossing = x[..., d] < box_min[d]


def periodized_diff(x, box_size, lees_edwards_strain, gradient_direction=-1, v=None, shear_rate=None):
    """ Periodize separations x (and optionally velocities v) according to Lees-Edwards in a box with limits box_lim.

        x and v are arrays with shape (..., d), box_size is a vector
        of size (d) containing the size of containing box in each direction,
        lees_edwards_strain  and shear_rate are vectors of size d.

        Note that the Lees-Edwards strain and shear rate must be 0 along the
        gradient direction. 

        If v is given (with same shape than x), the shear rate should be given too.
    """

    if lees_edwards_strain[gradient_direction] != 0:
        raise RuntimeError("lees_edwards_strain[gradient_direction] != 0")

    g = gradient_direction

    box_size = np.asarray(box_size)
    box_half_size = 0.5*box_size

    x_periodized = np.array(x)
    if v is not None:
        v_periodized = np.array(v)

    # can be in 2 directions other than g
    lees_edwards_disp = np.array(lees_edwards_strain) * box_size[g]

    # start by the gradient direction, easier
    # gcrossing_shift contains the displacement to apply to x when it is out of the box in the g direction
    gcrossing_shift = np.array(lees_edwards_disp)
    gcrossing_shift[g] = box_size[g]


    # how many times we need to apply the shift
    crossings = np.trunc((x_periodized[..., g] + box_half_size[g]*np.sign(x_periodized[..., g]))/box_size[g])
    x_periodized -= np.multiply.outer(crossings, gcrossing_shift)
    if v is not None:
        shear_rate = np.array(shear_rate)
        v_periodized[...,:] -= np.multiply.outer(crossings, shear_rate*box_size[g])

    for d in range(x_periodized.shape[-1]):
        if d != g:
            crossings = np.trunc((x_periodized[..., d] + box_half_size[d]*np.sign(x_periodized[..., d]))/box_size[d])
            x_periodized[..., d] -= crossings*box_size[d]

    if v is not None:
        return x_periodized, v_periodized
    else:
        return x_periodized


def centered_box_lim(box_size):
    box_lim = np.column_stack((np.zeros(len(box_size)), box_size))
    box_lim = np.asarray(box_lim)
    # # [-L/2,L/2] in any direction
    box_lim -= np.mean(box_lim, axis=1)[:, np.newaxis]

    return box_lim


def get_positions(column_def, conf_data):
    """
        Get the particles positions in a configuration snapshot.
        
        Parameters
        ==========
        
        Starting from snapshot psnap from a "par" cllf.snapshot_file par_f:
        * column_def is given by the par_f.column_def()
        * conf_data is psnap[1]
        
        
        Returns
        =======
        
        A Nxd position array.

    """
    x, z = conf_data[:, column_def['position x']], conf_data[:, column_def['position z']]
    try:
        y = conf_data[:, column_def['position y']]
        pos = np.column_stack((x,y,z))
    except KeyError:
        pos = np.column_stack((x,z))

    return pos


def get_velocities(column_def, conf_data, nonaffine=False, shear_rate=None):
    """
        Get the particles velocities in a configuration snapshot.
        
        Parameters
        ==========
        
        Starting from snapshot psnap from a "par" cllf.snapshot_file par_f:
        * column_def is given by the par_f.column_def()
        * conf_data is psnap[1]
        
        nonaffine: if False returns the full velocity, otherwise only the non-affine velo.
        If True, need to specify the shear rate, a vector of size d

        
        Returns
        =======
        
        A Nxd velocity array.

    """
    vslice = column_def['velocity (x, y, z)']
    vel = conf_data[:, vslice]
    dim = 3 if 'position y' in column_def else 2
    if dim==2:
        vel = vel[:, [0, 2]]

    if nonaffine:
        pos = get_positions(column_def, conf_data)
        vel[:,0] -= shear_rate[0]*pos[:,-1]
        if dim==3:
            vel[:,1] -= shear_rate[1]*pos[:,-1]

    return vel


def get_interacting_pairs(int_column_def, int_data):
    p1_col = int_column_def['particle 1 label']
    p2_col = int_column_def['particle 2 label']
    p1 = int_data[:, p1_col].astype(np.int)
    p2 = int_data[:, p2_col].astype(np.int)

    return p1, p2


def get_system_size(simu_meta):
    """
        Get the particles positions in a configuration snapshot.
        
        Parameters
        ==========
        
        Starting a cllf.snapshot_file f:
        * simu_meta is f.meta_data()
        
        
        Returns
        =======
        
        A vector of size d, [lx, lz] if d==2 or [lx, ly, lz] if d==3.
    """
    box_size = np.array([np.float(simu_meta['Lx']), 
                         np.float(simu_meta['Ly']),
                         np.float(simu_meta['Lz'])])
    if box_size[1] > 0:
        return box_size
    else:
        return box_size[[0,2]]


def get_LeesEdwards_strain(conf_meta, box_size):
    """
        Get the Lees-Edwards BC strain for a snapshot.
        
        Parameters
        ==========
        
        Starting from snapshot snap from a cllf.snapshot_file f:
        * conf_meta is snap[0]
        * box_size is given by get_system_size(f.meta_data())
        
        Returns
        =======
        
        A vector of size d with strain in x, z (d==2) or (x,y,z) (d==3).
    """
    try:
        sx, sy = conf_meta[b'shear disp x']/box_size[1], conf_meta[b'shear disp y']/box_size[1]
        lees_edwards_strain = [sx, sy, 0] if dim == 3 else [sx, 0]
    except KeyError:
        # print("Warning: data format does not record y strain.")
        sx = conf_meta[b'shear disp']/box_size[1]
        lees_edwards_strain = [sx, 0]

    return lees_edwards_strain


def pair_distance(x,  box_size, lees_edwards_strain, gradient_direction=-1):
    """ Get the pairwise separation matrix from an array of positions x
        according to Lees-Edwards in a box with limits box_lim.

        x is an array with shape (N, d), box_lim is a vector with size (d)
        containing the min and max box coordinates in each direction,
        and lees_edwards_strain is a vector of size d.

        Note that the Lees-Edwards strain must be 0
        along the gradient direction.
    """
    lees_edwards_strain = np.asarray(lees_edwards_strain)

    pairwise_separation = x[:, np.newaxis, :] - x
    pd_sep = periodized_diff(pairwise_separation,
                              box_size,
                              lees_edwards_strain,
                              gradient_direction)
    return pd_sep


def pair_separation_snapshot(column_def, conf_meta, conf_data, box_size, velocity=False):
    """
        Get the separation between all pairs of particles in a configuration snapshot.
        
        Parameters
        ==========
        
        Starting from snapshot psnap from a "par" cllf.snapshot_file par_f:
        * column_def is given by the par_f.column_def()
        * conf_meta is psnap[0]
        * conf_data is psnap[1]
        * box_size is given by get_system_size(par_f.meta_data())
        
        
        Returns
        =======
        
        A NxNxd separation array.

    """
    pos = get_positions(column_def, conf_data)

    dim = pos.shape[-1]

    lees_edwards_strain = get_LeesEdwards_strain(conf_meta, box_size)
    
    gradient_direction = -1
    pairwise_separation = pos[:, np.newaxis, :] - pos
    
    if velocity:
        vel = get_velocities(column_def, conf_data)
        pairwise_velocity = vel[:, np.newaxis, :] - vel
        
        if dim == 2:
            shear_rate = np.array([conf_meta[b'shear rate'], 0])
        else:
            theta = conf_meta[b'theta']
            shear_rate = conf_meta[b'shear rate']*np.array([np.cos(theta), 
                                                            np.sin(theta), 
                                                            0])

        pd_sep, pd_vel = periodized_diff(pairwise_separation, 
                                         box_size, 
                                         lees_edwards_strain, 
                                         gradient_direction=-1, 
                                         v=pairwise_velocity,
                                         shear_rate=shear_rate)
        return pd_sep, pd_vel
    else:
        pd_sep = periodized_diff(pairwise_separation, box_size, lees_edwards_strain, gradient_direction=-1)
        return pd_sep


def pair_velocity_interactions(int_column_def, int_meta, int_data, conf_column_def, conf_meta, conf_data, box_size):
    """
        Get the relative velocities for all interacting particle pairs.
        
        Parameters
        ==========
        
        Starting from correponding snapshots isnap and psnap from an "int" cllf.snapshot_file int_f and a "par" cllf.snapshot_file par_f:
        * int_column_def is given by the int_f.column_def()
        * int_meta is isnap[0]
        * int_data is isnap[1]
        * conf_column_def is given by the par_f.column_def()
        * conf_meta is psnap[0]
        * conf_data is psnap[1]
        * box_size is given by get_system_size(par_f.meta_data())
        
        Returns
        =======
        
        Two arrays, containing:
        * separation vector from p0 to p1 (that is, pos[p1] - pos[p0], taking into account PBC)
        * pairwise velocity vel[p1] - vel[p0], taking into account PBC
    """
    
    pos = get_positions(conf_column_def, conf_data)
    dim = pos.shape[-1]
    
    lees_edwards_strain = get_LeesEdwards_strain(conf_meta, box_size)

    gradient_direction = -1
    p1, p2 = get_interacting_pairs(int_column_def, int_data)
    
    pos_col = slice(conf_column_def['position x'], conf_column_def['position z']+1)
    p1_pos = conf_data[p1, pos_col]
    p2_pos = conf_data[p2, pos_col]
    
    if dim == 2:
        shear_rate = np.array([conf_meta[b'shear rate'], 0])
    else:
        theta = conf_meta[b'theta']
        shear_rate = conf_meta[b'shear rate']*np.array([np.cos(theta), 
                                                        np.sin(theta), 
                                                        0])

    vel = get_velocities(conf_column_def, conf_data)
    p1_vel = vel[p1]
    p2_vel = vel[p2]
    
    pair_sep = p2_pos - p1_pos
    pair_vel = p2_vel - p1_vel
    dim = p1_pos.shape[-1]
    
    pd_sep, pd_vel = periodized_diff(pair_sep, 
                                     box_size, 
                                     lees_edwards_strain, 
                                     gradient_direction=-1, 
                                     v=pair_vel, 
                                     shear_rate=shear_rate)
    return pd_sep, pd_vel


def get_interaction_end_points(int_snapshot,
                               par_snapshot,
                               int_cols,
                               par_cols):
    """
        For each interaction in f, get the position of the particles involved.
        Positions of every particle given in p. Return is NOT periodized.

        Returns an array containing x1,y1,z1,x2,y2,z2 for each interaction.
    """
    p1_idx = du.matching_uniq(int_cols, ['label', '1'])[1]
    p2_idx = du.matching_uniq(int_cols, ['label', '2'])[1]
    try:
        pos_idx = du.matching_uniq(par_cols, 'position')[1]
    except Exception:
        pos_idx = slice(du.matching_uniq(par_cols, ['position', 'x'])[1],
                        du.matching_uniq(par_cols, ['position', 'z'])[1]+1)
    # for each interaction: the particle indices
    part1 = int_snapshot[:, p1_idx].astype(np.int)
    part2 = int_snapshot[:, p2_idx].astype(np.int)

    # for each interaction: the particle positions
    r1 = par_snapshot[part1, pos_idx].astype(np.float)
    r2 = par_snapshot[part2, pos_idx].astype(np.float)

    return np.hstack((r1, r2))
