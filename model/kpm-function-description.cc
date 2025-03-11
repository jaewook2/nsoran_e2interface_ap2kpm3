/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2022 Northeastern University
 * Copyright (c) 2022 Sapienza, University of Rome
 * Copyright (c) 2022 University of Padova
 * Copyright (c) 2024 Orange Innovation Egypt
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Andrea Lacava <thecave003@gmail.com>
 *		   Tommaso Zugno <tommasozugno@gmail.com>
 *		   Michele Polese <michele.polese@gmail.com>
 *       Mostafa Ashraf <mostafa.ashraf.ext@orange.com>
 *       Aya Kamal <aya.kamal.ext@orange.com>
 *       Abdelrhman Soliman <abdelrhman.soliman.ext@orange.com>
 */

#include <ns3/kpm-function-description.h>
#include <ns3/asn1c-types.h>
#include <ns3/log.h>

extern "C" {
#include "RIC-EventTriggerStyle-Item.h"
#include "RIC-ReportStyle-Item.h"
}

const char* performance_measurements[] = {
  "DRB.RlcSduTransmittedVolumeDL",
  "DRB.RlcSduTransmittedVolumeUL",
  "DRB.PerDataVolumeDLDist.Bin",
  "DRB.PerDataVolumeULDist.Bin",
  "DRB.RlcPacketDropRateDLDist",
  "DRB.PacketLossRateULDist",
  "L1M.DL-SS-RSRP.SSB",
  "L1M.DL-SS-SINR.SSB",
  "L1M.UL-SRS-RSRP"
  };

int NUMBER_MEASUREMENTS = 9;

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("KpmFunctionDescription");

KpmFunctionDescription::KpmFunctionDescription ()
{
  E2SM_KPM_RANfunction_Description_t *descriptor = new E2SM_KPM_RANfunction_Description_t ();
  FillAndEncodeKpmFunctionDescription (descriptor);
  ASN_STRUCT_FREE_CONTENTS_ONLY (asn_DEF_E2SM_KPM_RANfunction_Description, descriptor);
  delete descriptor;
}

KpmFunctionDescription::~KpmFunctionDescription ()
{
  free (m_buffer);
  m_size = 0;
}

void
KpmFunctionDescription::Encode (E2SM_KPM_RANfunction_Description_t *descriptor)
{
  if (false) {
    asn_codec_ctx_t *opt_cod = 0; // disable stack bounds checking
    // encode the structure into the e2smbuffer
    asn_encode_to_new_buffer_result_s encodedMsg = asn_encode_to_new_buffer (
        opt_cod, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2SM_KPM_RANfunction_Description, descriptor);

    if (encodedMsg.result.encoded < 0)
      {
        printf("I am Hereeeeeeeeeeeeeeeeeeeeeeee");
        
        NS_FATAL_ERROR ("Error during the encoding of the RIC Indication Header, errno: "
                        << strerror (errno) << ", failed_type " << encodedMsg.result.failed_type->name
                        << ", structure_ptr " << encodedMsg.result.structure_ptr);
      }

    m_buffer = encodedMsg.buffer;
    m_size = encodedMsg.result.encoded;
  } else {
    asn_codec_ctx_t *opt_cod = 0; // disable stack bounds checking
    // encode the structure into the e2smbuffer
    Bytee_array_t ba = {.buf = (uint8_t *) malloc (2048), .len = 2048};
    asn_enc_rval_t encodedMsg = asn_encode_to_buffer(
        opt_cod, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2SM_KPM_RANfunction_Description, descriptor,ba.buf,ba.len);

    if (encodedMsg.encoded < 0)
      {
        printf("Error");
        
        NS_FATAL_ERROR ("Error during the encoding of the RIC Indication Header, errno: "
                        << strerror (errno) << ", failed_type " << encodedMsg.failed_type->name
                        << ", structure_ptr " << encodedMsg.structure_ptr);
      }

    m_buffer = ba.buf;
    m_size = encodedMsg.encoded;
  }
}


OCTET_STRING_t KpmFunctionDescription::cp_str_to_ba(const char* str)
{
    
    assert(str != NULL);
    const size_t sz = strlen(str);
    OCTET_STRING_t asn = {0};

    asn.buf = (uint8_t*)calloc(sz,sizeof(uint8_t));
      assert(asn.buf != NULL && "Memory exhausted");

      memcpy(asn.buf, str, sz);

    // asn.buf = (uint8_t*) calloc(sizeof(x) + 1, sizeof(char));
    // memcpy(asn.buf,&x,sizeof(x));
    // asn.size = sizeof(x);

    return asn;
}

// Update e2ap v2
void
KpmFunctionDescription::FillAndEncodeKpmFunctionDescription (
    E2SM_KPM_RANfunction_Description_t *ranfunc_desc)
{
  
  uint8_t* short_name = (uint8_t*)"ORAN-E2SM-KPM";
  uint8_t* func_desc = (uint8_t*)"KPM Monitor";
  uint8_t* e2sm_odi = (uint8_t*)"OID123";

  LOG_I("short_name: %s, func_desc: %s, e2sm_odi: %s", short_name, func_desc, e2sm_odi);
  ASN_STRUCT_RESET(asn_DEF_E2SM_KPM_RANfunction_Description, ranfunc_desc);
  
  ranfunc_desc->ranFunction_Name.ranFunction_ShortName.size = strlen((char*)short_name);
  ranfunc_desc->ranFunction_Name.ranFunction_ShortName.buf =
      (uint8_t*)calloc(strlen((char*)short_name), sizeof(uint8_t));
  memcpy(ranfunc_desc->ranFunction_Name.ranFunction_ShortName.buf, short_name,
         ranfunc_desc->ranFunction_Name.ranFunction_ShortName.size);

  ranfunc_desc->ranFunction_Name.ranFunction_Description.buf =
      (uint8_t*)calloc(1, strlen((char*)func_desc));
  memcpy(ranfunc_desc->ranFunction_Name.ranFunction_Description.buf, func_desc,
        strlen((char*)func_desc));
  ranfunc_desc->ranFunction_Name.ranFunction_Description.size = strlen((char*)func_desc);
  ranfunc_desc->ranFunction_Name.ranFunction_Instance = (long *)calloc(1, sizeof(long));
  *ranfunc_desc->ranFunction_Name.ranFunction_Instance = 1;

  ranfunc_desc->ranFunction_Name.ranFunction_E2SM_OID.buf =
  (uint8_t*)calloc(1, strlen((char*)e2sm_odi));
  memcpy(ranfunc_desc->ranFunction_Name.ranFunction_E2SM_OID.buf, e2sm_odi,
     strlen((char*)e2sm_odi));
  ranfunc_desc->ranFunction_Name.ranFunction_E2SM_OID.size = strlen((char*)e2sm_odi);

  LOG_I("Initialize event trigger style list structure");


  //Periodic report
  RIC_EventTriggerStyle_Item_t* trigger_style =
  (RIC_EventTriggerStyle_Item_t*)calloc(1, sizeof(RIC_EventTriggerStyle_Item_t));
  trigger_style->ric_EventTriggerStyle_Type = 1;
  uint8_t* style_name = (uint8_t*)"Periodic Report";
  trigger_style->ric_EventTriggerStyle_Name.buf = (uint8_t*)calloc(1, strlen((char*)style_name));
  memcpy(trigger_style->ric_EventTriggerStyle_Name.buf, style_name, strlen((char*)style_name));
  trigger_style->ric_EventTriggerStyle_Name.size = strlen((char*)style_name);
  trigger_style->ric_EventTriggerFormat_Type = 1;

  ranfunc_desc->ric_EventTriggerStyle_List =
    (E2SM_KPM_RANfunction_Description::E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List*)
        calloc(1, sizeof(E2SM_KPM_RANfunction_Description::E2SM_KPM_RANfunction_Description__ric_EventTriggerStyle_List));

  int ret = ASN_SEQUENCE_ADD(&ranfunc_desc->ric_EventTriggerStyle_List->list, trigger_style);

  LOG_I("Initialize report style structure");

  MeasurementInfo_Action_List_t* measInfo_Action_List =
      (MeasurementInfo_Action_List_t*)calloc(1, sizeof(MeasurementInfo_Action_List_t));
  // To be update for measurement
  for (int i = 0; i < NUMBER_MEASUREMENTS; i++) {  //for (int i = 0; i < NUMBER_MEASUREMENTS; i++) {
    uint8_t* metrics = (uint8_t *)performance_measurements[i];
    MeasurementInfo_Action_Item_t* measItem =(MeasurementInfo_Action_Item_t*)calloc(1, sizeof(MeasurementInfo_Action_Item_t));
    measItem->measName.buf = (uint8_t*)calloc(1, strlen((char*)metrics));
    memcpy(measItem->measName.buf, metrics, strlen((char*)metrics));
    
    measItem->measName.size = strlen((char*)metrics);

    measItem->measID = (MeasurementTypeID_t*)calloc(1, sizeof(MeasurementTypeID_t));
    *measItem->measID = i+1;

    ASN_SEQUENCE_ADD(&measInfo_Action_List->list, measItem);
  }

  RIC_ReportStyle_Item_t *report_style = (RIC_ReportStyle_Item_t *) calloc (1, sizeof (RIC_ReportStyle_Item_t));
  report_style->ric_ReportStyle_Type = 4;
// O-CU-CP Measurement Container for the EPC connected deployment
  uint8_t *reportStyleNameBuffer = (uint8_t *) "O-CU-CP";
  report_style->ric_ReportStyle_Name.buf = (uint8_t *) calloc (1, strlen ((char *) reportStyleNameBuffer));
  memcpy (report_style->ric_ReportStyle_Name.buf, reportStyleNameBuffer, strlen ((char *) reportStyleNameBuffer));
  report_style->ric_ReportStyle_Name.size = strlen ((char *) reportStyleNameBuffer);
  report_style->ric_IndicationHeaderFormat_Type = 1;  
  report_style->ric_IndicationMessageFormat_Type = 3;
  report_style->ric_ActionFormat_Type = 4; 
  report_style->measInfo_Action_List = *measInfo_Action_List;




 ranfunc_desc->ric_ReportStyle_List =
       (E2SM_KPM_RANfunction_Description::E2SM_KPM_RANfunction_Description__ric_ReportStyle_List *)
           calloc (1, sizeof (E2SM_KPM_RANfunction_Description::
                                  E2SM_KPM_RANfunction_Description__ric_ReportStyle_List));

  ASN_SEQUENCE_ADD (&ranfunc_desc->ric_ReportStyle_List->list, report_style);

  Encode (ranfunc_desc);

  NS_LOG_INFO (xer_fprint (stderr, &asn_DEF_E2SM_KPM_RANfunction_Description, ranfunc_desc));
}

} // namespace ns3
