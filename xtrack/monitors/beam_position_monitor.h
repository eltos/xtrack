// ##################################
// Beam Monitor
// 
// Author: Rahul Singh, Philipp Niedermayer
// Date: 2023-06-10
// ##################################


#ifndef XTRACK_BEAM_POSITION_MONITOR_H
#define XTRACK_BEAM_POSITION_MONITOR_H

#if !defined( C_LIGHT )
    #define   C_LIGHT ( 299792458.0 )
#endif /* !defined( C_LIGHT ) */

/*gpufun*/
void BeamPositionMonitor_track_local_particle(BeamPositionMonitorData el, LocalParticle* part0){

    // get parameters
    int64_t const start_at_turn = BeamPositionMonitorData_get_start_at_turn(el);
    int64_t particle_id_start = BeamPositionMonitorData_get_particle_id_start(el);
    int64_t particle_id_stop = particle_id_start + BeamPositionMonitorData_get_num_particles(el);
    double const frev = BeamPositionMonitorData_get_frev(el);
    double const sampling_frequency = BeamPositionMonitorData_get_sampling_frequency(el);
    BeamPositionMonitorRecord record = BeamPositionMonitorData_getp_data(el);


    //start_per_particle_block(part0->part)

        int64_t particle_id = LocalParticle_get_particle_id(part);
        if (particle_id_start <= particle_id && particle_id < particle_id_stop){

            // zeta is the absolute path length deviation from the reference particle: zeta = (s - beta0*c*t)
            // but without limits, i.e. it can exceed the circumference (for coasting beams)
            // as the particle falls behind or overtakes the reference particle
            double const zeta = LocalParticle_get_zeta(part);
            double const at_turn = LocalParticle_get_at_turn(part);
            double const beta0 = LocalParticle_get_beta0(part);

            // compute sample index and store data in buffer
            int64_t slot = roundf(sampling_frequency * ( (at_turn-start_at_turn)/frev - zeta/beta0/C_LIGHT ));
            BeamPositionMonitorRecord_set_slot(record, particle_id-particle_id_start, slot);
            BeamPositionMonitorRecord_set_x(record, particle_id-particle_id_start, LocalParticle_get_x(part));
            BeamPositionMonitorRecord_set_y(record, particle_id-particle_id_start, LocalParticle_get_y(part));

        }

	//end_per_particle_block

}

#endif

