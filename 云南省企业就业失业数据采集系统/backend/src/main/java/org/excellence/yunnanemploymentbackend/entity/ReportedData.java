package org.excellence.yunnanemploymentbackend.entity;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import lombok.Data;
import lombok.NoArgsConstructor;
import org.apache.tomcat.util.bcel.Const;

@Data
@NoArgsConstructor
@Entity
public class ReportedData {
    public static class Status {
        public final static String ALL_APPROVED = "ALL_APPROVED"; // 省和市全部审核通过
        public final static String ONLY_CITY = "ONLY_CITY"; // 市通过省未通过
        public final static String WAITING_APPROVED = "WAITING_APPROVED"; // 市和省都还没审
        public final static String REJECTED_BY_CITY = "REJECTED_BY_CITY"; // 被市退回
        public final static String REJECTED_BY_PROVINCE = "REJECTED_BY_PROVINCE"; // 被省退回
    }
    @Id @GeneratedValue(strategy = GenerationType.AUTO)
    private Integer reportedDataId;
    private String enterpriseUserId;
    private String status;
    private Integer employedNumDuringFilingPeriod;
    private Integer employedNumDuringSurveyPeriod;
    private String otherReasons;
    private String reducedType;
    private String mainReason;
    private String mainReasonDescription;
    private String secondaryReason;
    private String secondaryReasonDescription;
    private String thirdReason;
    private String thirdReasonDescription;
}
