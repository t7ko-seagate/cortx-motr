/* -*- C -*- */
/*
 * Copyright (c) 2017-2020 Seagate Technology LLC and/or its Affiliates
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For any questions about this software or licensing,
 * please email opensource@seagate.com or cortx-questions@seagate.com.
 *
 */


#define M0_TRACE_SUBSYSTEM M0_TRACE_SUBSYS_DIXCM
#include "lib/trace.h"
#include "fop/fop.h"
#include "cm/cp.h"

#include "dix/cm/cm.h"
#include "dix/cm/dix_cp_onwire.h"
#include "dix/cm/dix_cp_onwire_xc.h"

struct m0_fop_type m0_dix_repair_cpx_fopt;
struct m0_fop_type m0_dix_repair_cpx_reply_fopt;
extern struct m0_cm_type dix_repair_cmt;

static int dix_repair_cp_fom_create(struct m0_fop *fop, struct m0_fom **m,
				    struct m0_reqh *reqh);

const struct m0_fom_type_ops dix_repair_cp_fom_type_ops = {
        .fto_create = dix_repair_cp_fom_create
};

static int dix_repair_cp_fom_create(struct m0_fop *fop, struct m0_fom **m,
				    struct m0_reqh *reqh)
{
	struct m0_fop *r_fop;
	int            rc;

	r_fop = m0_fop_reply_alloc(fop, &m0_dix_repair_cpx_reply_fopt);
	if (r_fop == NULL)
		return M0_ERR(-ENOMEM);
	rc = m0_cm_cp_fom_create(fop, r_fop, m, reqh);

	return M0_RC(rc);
}

/**
 * Initialises DIX rerepair FOP and reply FOP types.
 *
 * @see m0_dix_cpx_init()
 */
M0_INTERNAL void m0_dix_cm_repair_cpx_init(void)
{
	m0_dix_cpx_init(&m0_dix_repair_cpx_fopt,
			&dix_repair_cp_fom_type_ops,
			M0_DIX_CM_REPAIR_CP_OPCODE,
			"DIX_Repair_copy_packet", m0_dix_cpx_xc,
			M0_RPC_MUTABO_REQ,
			&dix_repair_cmt);
	m0_dix_cpx_init(&m0_dix_repair_cpx_reply_fopt,
			&dix_repair_cp_fom_type_ops,
			M0_DIX_CM_REPAIR_CP_REP_OPCODE,
			"DIX_Repair_copy_packet_reply",
			m0_cpx_reply_xc, M0_RPC_ITEM_TYPE_REPLY,
			&dix_repair_cmt);
}

/**
 * Finalises DIX repair FOP and reply FOP types.
 *
 * @see m0_dix_cpx_fini()
 */
M0_INTERNAL void m0_dix_cm_repair_cpx_fini(void)
{
        m0_dix_cpx_fini(&m0_dix_repair_cpx_fopt);
        m0_dix_cpx_fini(&m0_dix_repair_cpx_reply_fopt);
}

#undef M0_TRACE_SUBSYSTEM

/*
 *  Local variables:
 *  c-indentation-style: "K&R"
 *  c-basic-offset: 8
 *  tab-width: 8
 *  fill-column: 80
 *  scroll-step: 1
 *  End:
 */
