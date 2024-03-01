package org.excellence.yunnanemploymentbackend.entity;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@Entity
public class EnterpriseInfo {
    public static class Status {
        public final static Integer IN_REVIEWING = 0;
        public final static Integer IS_APPROVED = 1;
        public final static Integer IS_REJECTED = 2;
    }
    @Id @GeneratedValue(strategy = GenerationType.AUTO)
    private Integer enterpriseInfoId;
    private String enterpriseUserId;
    private Integer status; // 0: 审核中; 1: 审核通过; 2: 被拒绝
    private String cityOfAffiliation; // 所属的市
    private String code; // 组织机构代码
    private String name; // 企业名称
    private String property; // 企业性质
    private String industry; // 所属行业
    private String mainBusiness; // 主要经营业务
    private String contact; // 联系人
    private String address; // 联系地址
    private String postalCode; // 邮政编码
    private String phoneNumber; // 联系电话
    private String fax;
    private String email;
    private Integer recordedPeopleNumber;
}
